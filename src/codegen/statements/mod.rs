use std::rc::Rc;
use ast::{Statement, Statements};
use vm::{scope, types, Op, get_type_ref_from_string};
use vm::function::{Function, VMFunction};
use super::{evaluate_expr};

pub fn gen_statement(s: &Statement, scope: &mut scope::Scope, ops: &mut Vec<Op>) {
    match s {
        &Statement::FunctionDecl(ref func_decl) => {
            let mut func_scope = scope::Scope::new();
            func_scope.allocate_local(types::NONE_TYPE.clone());
            let mut argument_names = Vec::new();
            for ref a in &(func_decl.arguments) {
                let typ = get_type_ref_from_string(&a.typ);
                func_scope.add_local(&a.name, typ);
                argument_names.push(a.name.clone());
            }
            let mut func_ops = Vec::new();
            for s in &func_decl.statements {
                gen_statement(s, &mut func_scope, &mut func_ops);
            }
            scope.add_function(func_decl.name.clone(), Rc::new(Function::VMFunction(VMFunction{
                name: func_decl.name.clone(),
                argument_names: argument_names,
                return_typ: types::get_type_ref_from_string(&func_decl.typ),
                scope: func_scope,
                ops: func_ops
            })));
        },
        &Statement::Return(ref expr) => {
            let op = Op::Return{register: evaluate_expr(expr, scope, ops).index};
            ops.push(op);
        },
        &Statement::Expr(ref expr) => {evaluate_expr(expr, scope, ops);},
        &Statement::Declaration(ref d) => {
            let result = evaluate_expr(&(d.expression), scope, ops);
            let object = scope.add_local(&(d.name.clone()), result.typ);
            ops.push(Op::Assign{source: result.index, target: object.index});
        },
        &Statement::Assignment(ref d) => {
            match scope.get_local(&(d.name)) {
                Some(object) => {
                    let result = evaluate_expr(&(d.expression), scope, ops);
                    if object.typ == result.typ {
                        ops.push(Op::Assign{source: result.index, target: object.index});
                    } else {
                        panic!(format!("mismatched types for assignment to {0}. Expected {1}, got {2}", d.name, object.typ, result.typ));
                    }
                }
                _ => {
                    panic!(format!("unable to assign to undeclared variable {0}", d.name));
                }
            }
        },
        &Statement::Import(ref i) {
            ops.push(Op::ModuleLoadValue{module_name: Rc::new(i.module_name), name: Rc::new(name)});
        },
        &Statement::While(ref w) => {
            let start_index = ops.len();
            let result_obj = evaluate_expr(&(w.condition), scope, ops);
            let cond_index = ops.len();
            ops.push(Op::Noop{});
            gen_statement_list(&(w.block), scope, ops);
            // go back to the condition statement, to
            // see if we should loop again.
            ops.push(Op::Goto{position: start_index});
            let end_of_while_position = ops.len();
            ops[cond_index] = Op::Branch{condition: result_obj.index, if_false: end_of_while_position};
        }
    };
}

pub fn gen_statement_list(statements: &Statements, scope: &mut scope::Scope, ops: &mut Vec<Op>) {
    for s in statements {
        gen_statement(s, scope, ops);
    }
}
