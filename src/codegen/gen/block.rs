use vm::{VM};
use ast::{Statements};
use codegen::{Block, Context, CGResult, CGError};
use super::gen_statement;

pub fn gen_block(vm: &mut VM, statements: &Statements) -> CGResult<Block> {
    let mut c = Context::new(vm);
    for ref s in statements {
        gen_statement(&mut c, s);
    }
    c.block.finalize();
    Ok(c.block)
}
