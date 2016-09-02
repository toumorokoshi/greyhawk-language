extern crate yaml_rust;
use super::vm;
mod block;
mod context;
mod expressions;
mod gen;
mod statements;
use ast::{Statement, Expression};
use vm::{scope, types, Op};
use std::rc::Rc;
pub use self::statements::{
    gen_statement,
    gen_statement_list
};
pub use self::context::{Context};
pub use self::block::{gen_block, Block};

// we don't build modules directly, as they are sometimes evaluated on load instead.
// thus, a module builder is created instead.
pub fn gen_module_builder(vm: &mut vm::VM, statements: &Vec<Box<Statement>>) -> vm::ModuleBuilder {
    let block = gen_block(vm, statements);
    vm::ModuleBuilder{scope: Rc::new(block.scope), ops: block.ops}
}

pub fn generate_ops(vm: &mut vm::VM, statements: &Vec<Box<Statement>>) -> vm::Function {
    let block = gen_block(vm, statements);
    vm::ModuleBuilder{scope: Rc::new(block.scope), ops: block.ops}
    return vm::Function::VMFunction(vm::VMFunction {
        name: String::from("__main__"),
        argument_names: vec![],
        scope: block.scope,
        ops: block.ops,
        return_typ: types::NONE_TYPE.clone(),
    });
}

pub fn evaluate_expr(vm: &mut vm::VM, expr: &Expression, scope: &mut scope::Scope, ops: &mut Vec<vm::ops::Op>) -> scope::LocalObject {
    match expr {
        &Expression::ArrayCreate(ref ac) =>
            expressions::gen_array(vm, &ac, scope, ops),
        &Expression::Condition(ref c) =>
            expressions::gen_condition(vm, &c, scope, ops),
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
            ops.push(Op::StringLoad{register: obj.index, constant: value.clone()});
            obj
        },
        &Expression::ItemGet(ref ig) =>
            expressions::gen_index_get(vm, &ig, scope, ops),
        &Expression::ItemSet(ref is) =>
            expressions::gen_index_set(vm, &is, scope, ops),
        &Expression::Symbol(ref value) => {
            match scope.get_local(&(value.clone())) {
                Some(obj) => obj,
                None => { panic!(format!("unable to find symbol {}", value));}
            }
        },
        &Expression::BinOp(ref op) => expressions::generate_binop(vm, op, scope, ops),
        &Expression::Call{ref name, args: ref arg_expressions} => {
            let func = scope.get_function(name);
            let mut args = Vec::new();
            for e in arg_expressions {
                args.push(evaluate_expr(vm, &e, scope, ops));
            }
            let ret_val = scope.allocate_local(func.return_type());
            ops.push(Op::Call{func: func, args: args, target: ret_val.index});
            ret_val
        }
    }
}
