extern crate yaml_rust;
mod expressions;
use ast::Statement;
use super::vm;
use vm::Op;
use vm::scope;
use vm::types;
use yaml_rust::Yaml;
use std::rc::Rc;

// pub use self::yaml::to_yaml;

pub fn generate_ops(statements: &Vec<Statement>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    for statement in statements {
        statement.evaluate(&mut scope, &mut ops);
    }
    return vm::Function::VMFunction(vm::VMFunction {
        scope: scope,
        ops: ops
    });
}

pub fn evaluate_stat(statement: Statement, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) {
    match statement {
        Statement::FunctionDecl{name, statements} => {
        },
        Statement::Return(expr) => ops.push(Op::Return{register: evaluate_expr(expr)}),
        Statement::Expr(expr) => evaluate_expr(expr, scope, ops),
    }
}

pub fn evaluate_expr(expr: Expression, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) -> scope::LocalObject {
    match expr {
        Expression::ConstInt{value} => {
            let obj = scope.allocate_local(types::get_int_type());
            ops.push_back(Op::IntLoad{register: obj.index, constant: value});
            obj
        },
        Expression::ConstFloat{value} => {
            let obj = scope.allocate_local(types::get_float_type());
            ops.push_back(Op::FloatLoad{register: obj.index, constant: value});
            obj
        },
        Expression::BinOp(op) => expressions::generate_binop(op, scope, instructions),
        Expression::Call{name, arg} => {
            let func = scope.get_function(name);
            let args = vec![evaluate_expr(arg)];
            let ret_val = scope.allocate_local(func.return_type());
            ops.push_back(Op::Call{func: func, args: args, target: ret_val});
        }
    }
}
