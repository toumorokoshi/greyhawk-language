use super::vm;
mod block;
mod context;
mod error;
mod expression;
mod function;
mod statement;
use ast::{Statement};
use std::rc::Rc;
pub use self::function::{generate_function};
pub use self::statement::{
    gen_statement,
    gen_statement_list,
};
pub use self::expression::{gen_expression};
pub use self::context::{Context};
pub use self::block::{gen_block, Block};
pub use self::error::{CGError, CGResult};

// we don't build modules directly, as they are sometimes evaluated on load instead.
// thus, a module builder is created instead.
pub fn gen_module_builder(vm: &mut vm::VM, statements: &Vec<Box<Statement>>) -> CGResult<vm::ModuleBuilder> {
    let block = try!(gen_block(vm, statements));
    Ok(vm::ModuleBuilder{scope: Rc::new(block.scope), ops: block.ops})
}
