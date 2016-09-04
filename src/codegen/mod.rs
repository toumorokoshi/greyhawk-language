use super::vm;
mod block;
mod context;
mod error;
mod gen;
use ast::{Statement, Expression};
use vm::{scope, types, Op};
use std::rc::Rc;
pub use self::gen::{
    gen_statement,
    gen_statement_list,
    gen_block
};
pub use self::context::{Context};
pub use self::block::Block;
pub use self::error::{CGError, CGResult};

// we don't build modules directly, as they are sometimes evaluated on load instead.
// thus, a module builder is created instead.
pub fn gen_module_builder(vm: &mut vm::VM, statements: &Vec<Box<Statement>>) -> CGResult<vm::ModuleBuilder> {
    let block = try!(gen_block(vm, statements));
    Ok(vm::ModuleBuilder{scope: Rc::new(block.scope), ops: block.ops})
}

pub fn generate_ops(vm: &mut vm::VM, statements: &Vec<Box<Statement>>) -> CGResult<vm::Function> {
    let block = try!(gen_block(vm, statements));
    Ok(vm::Function::VMFunction(vm::VMFunction {
        name: String::from("__main__"),
        argument_names: vec![],
        scope: block.scope,
        ops: block.ops,
        return_typ: types::NONE_TYPE.clone(),
    }))
}
