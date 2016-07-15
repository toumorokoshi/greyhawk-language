extern crate yaml_rust;
mod expressions;
mod statements;
mod block;
use ast::Statement;
use ast::Expression;
use super::vm;
use vm::Op;
use vm::scope;
use vm::types;
use std::rc::Rc;
pub use self::statements::{
    gen_statement,
    gen_statement_list
};
pub use self::block::gen_block;

pub fn generate_ops(statements: &Vec<Box<Statement>>) -> vm::Function {
    let block = gen_block(statements);
    return vm::Function::VMFunction(vm::VMFunction {
        name: String::from("__main__"),
        scope: block.scope,
        ops: block.ops,
        return_typ: types::get_none_type(),
    });
}

pub fn evaluate_expr(expr: &Expression, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) -> scope::LocalObject {
    match expr {
        &Expression::Condition(ref c) =>
            expressions::gen_condition(&c, scope, ops),
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
