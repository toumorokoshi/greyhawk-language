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
        argument_names: vec![],
        scope: block.scope,
        ops: block.ops,
        return_typ: types::NONE_TYPE.clone(),
    });
}

pub fn evaluate_expr(expr: &Expression, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) -> scope::LocalObject {
    match expr {
        &Expression::ArrayCreate(ref ac) =>
            expressions::gen_array(&ac, scope, ops),
        &Expression::Condition(ref c) =>
            expressions::gen_condition(&c, scope, ops),
        &Expression::ConstInt{value} => {
            let obj = scope.allocate_local(types::INT_TYPE.clone());
            ops.push(Op::IntLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstFloat{value} => {
            let obj = scope.allocate_local(types::FLOAT_TYPE.clone());
            ops.push(Op::FloatLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstString{ref value} => {
            let obj = scope.allocate_local(types::STRING_TYPE.clone());
            ops.push(Op::StringLoad{register: obj.index, constant: Rc::new(value.clone())});
            obj
        },
        &Expression::ItemGet(ref ig) =>
            expressions::gen_index_get(&ig, scope, ops),
        &Expression::ItemSet(ref is) =>
            expressions::gen_index_set(&is, scope, ops),
        &Expression::Symbol(ref value) => {
            match scope.get_local(&(value.clone())) {
                Some(obj) => obj,
                None => { panic!(format!("unable to find symbol {}", value));}
            }
        },
        &Expression::BinOp(ref op) => expressions::generate_binop(op, scope, ops),
        &Expression::Call{ref name, args: ref arg_expressions} => {
            let func = scope.get_function(name);
            let mut args = Vec::new();
            for e in arg_expressions {
                args.push(evaluate_expr(&e, scope, ops));
            }
            let ret_val = scope.allocate_local(func.return_type());
            ops.push(Op::Call{func: func, args: args, target: ret_val.index});
            ret_val
        }
    }
}
