extern crate yaml_rust;
mod expressions;
use ast::Statement;
use ast::Expression;
use super::vm;
use vm::Op;
use vm::scope;
use vm::types;
use yaml_rust::Yaml;
use std::rc::Rc;

// pub use self::yaml::to_yaml;

pub fn generate_ops(statements: &Vec<Box<Statement>>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    for statement in statements {
        evaluate_stat(statement, &mut scope, &mut ops);
    }
    return vm::Function::VMFunction(vm::VMFunction {
        scope: scope,
        ops: ops,
        return_typ: types::get_none_type(),
    });
}

pub fn evaluate_stat(statement: &Statement, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) {
    match statement {
        &Statement::FunctionDecl{ref name, ref statements} => {},
        &Statement::Return(ref expr) => {
            let op = Op::Return{register: evaluate_expr(expr, scope, ops).index};
            ops.push(op);
        },
        &Statement::Expr(ref expr) => {evaluate_expr(expr, scope, ops);},
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
