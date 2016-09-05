use std::collections::HashMap;
use std::rc::Rc;
use ast::{Statement, Statements};
use vm::{Scope, types, Op, get_type_ref_from_string};
use vm::function::{Function, VMFunction};
use super::{gen_expression};
use codegen::{CGError, CGResult, Block, Context};

pub fn gen_statement(c: &mut Context, s: &Statement) -> CGResult<()> {
    match s {
        &Statement::FunctionDecl(ref func_decl) => {
            let mut func_scope = Scope::new(None);
            func_scope.allocate_local(types::NONE_TYPE.clone());
            let mut argument_names = Vec::new();
            for ref a in &(func_decl.arguments) {
                let typ = get_type_ref_from_string(&a.typ);
                func_scope.add_local(&a.name, typ);
                argument_names.push(a.name.clone());
            }
            let mut func_context = Context{
                block: Block {
                    ops: vec![],
                    scope: func_scope,
                    functions: HashMap::new(),
                },
                vm: c.vm
            };
            for s in &func_decl.statements {
                try!(gen_statement(&mut func_context, s));
            }
            c.block.scope.add_function(func_decl.name.clone(), Rc::new(Function::VMFunction(VMFunction{
                name: func_decl.name.clone(),
                argument_names: argument_names,
                return_typ: types::get_type_ref_from_string(&func_decl.typ),
                scope: func_context.block.scope,
                ops: func_context.block.ops
            })));
        },
        &Statement::Return(ref expr) => {
            let op = Op::Return{register: try!(gen_expression(c, expr)).index};
            c.block.ops.push(op);
        },
        &Statement::Expr(ref expr) => {try!(gen_expression(c, expr));},
        &Statement::Declaration(ref d) => {
            let result = try!(gen_expression(c, &(d.expression)));
            let object = c.block.scope.add_local(&(d.name.clone()), result.typ);
            c.block.ops.push(Op::Assign{source: result.index, target: object.index});
        },
        &Statement::Assignment(ref d) => {
            match c.block.scope.get_local(&(d.name)) {
                Some(object) => {
                    let result = try!(gen_expression(c, &(d.expression)));
                    if object.typ == result.typ {
                        c.block.ops.push(Op::Assign{source: result.index, target: object.index});
                    } else {
                        return Err(CGError::new(
                            &format!("mismatched types for assignment to {0}. Expected {1}, got {2}", d.name, object.typ, result.typ)
                        ));
                    }
                }
                _ => {
                    return Err(CGError::new(
                        &format!("unable to assign to undeclared variable {0}", d.name)
                    ));
                }
            }
        },
        &Statement::Import(ref i) => {
            let module = c.vm.load_module(&(i.module_name));
            match module.scope.get_local(&i.name) {
                Some(module_obj) => {
                    let obj = c.block.scope.add_local(&i.name, module_obj.typ);
                    c.block.ops.push(Op::ModuleLoadValue{
                        module_name: i.module_name.clone(),
                        name: i.name.clone(),
                        target: obj.index
                    });
                },
                None => {
                    return Err(CGError::new(
                        &format!("module {} does not have local {}", i.module_name, i.name)
                    ));
                }
            }
        },
        &Statement::While(ref w) => {
            let start_index = c.block.ops.len();
            let result_obj = try!(gen_expression(c, &(w.condition)));
            let cond_index = c.block.ops.len();
            c.block.ops.push(Op::Noop{});
            try!(gen_statement_list(c, &(w.block)));
            // go back to the condition statement, to
            // see if we should loop again.
            c.block.ops.push(Op::Goto{position: start_index});
            let end_of_while_position = c.block.ops.len();
            c.block.ops[cond_index] = Op::Branch{condition: result_obj.index, if_false: end_of_while_position};
        }
    };
    Ok(())
}

pub fn gen_statement_list(c: &mut Context, statements: &Statements) -> CGResult<()> {
    for s in statements {
        try!(gen_statement(c, s));
    }
    Ok(())
}
