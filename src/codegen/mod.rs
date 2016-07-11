extern crate yaml_rust;
mod expressions;
use ast::Statement;
use ast::Expression;
use super::vm;
use vm::Op;
use vm::scope;
use vm::types;
use vm::function::Function;
use vm::function::VMFunction;
use std::rc::Rc;

// pub use self::yaml::to_yaml;

pub fn generate_ops(statements: &Vec<Box<Statement>>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    for statement in statements {
        evaluate_stat(statement, &mut scope, &mut ops);
    }
    return vm::Function::VMFunction(vm::VMFunction {
        name: String::from("__main__"),
        scope: scope,
        ops: ops,
        return_typ: types::get_none_type(),
    });
}

pub fn evaluate_stat(statement: &Statement, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) {
    match statement {
        &Statement::FunctionDecl(ref func_decl) => {
            let mut func_scope = scope::Scope::new();
            func_scope.allocate_local(types::get_none_type());
            let mut func_ops = Vec::new();
            for s in &func_decl.statements {
                evaluate_stat(s, &mut func_scope, &mut func_ops);
            }
            scope.add_function(func_decl.name.clone(), Rc::new(Function::VMFunction(VMFunction{
                name: func_decl.name.clone(),
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
        }
    };
}

pub fn evaluate_expr(expr: &Expression, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) -> scope::LocalObject {
    match expr {
        &Expression::ConstInt{value} => {
            let obj = scope.allocate_local(types::get_int_type());
            ops.push(Op::IntLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstFloat{value} => {
            let obj = scope.allocate_local(types::get_float_type());
            ops.push(Op::FloatLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstString{ref value} => {
            let obj = scope.allocate_local(types::get_string_type());
            ops.push(Op::StringLoad{register: obj.index, constant: Rc::new(value.clone())});
            obj
        },
        &Expression::Symbol(ref value) => {
            match scope.get_local(&(value.clone())) {
                Some(obj) => obj,
                None => { panic!(format!("unable to find symbol {}", value));}
            }
        },
        &Expression::BinOp(ref op) => expressions::generate_binop(op, scope, ops),
        &Expression::Call{ref name, ref arg} => {
            let func = scope.get_function(name);
            let args = vec![evaluate_expr(&arg, scope, ops)];
            let ret_val = scope.allocate_local(func.return_type());
            ops.push(Op::Call{func: func, args: args, target: ret_val.index});
            ret_val
        }
    }
}
