mod statement;
use vm::{VM};
use ast::{Statements};
use super::{Block, Context};

pub fn gen_block(vm: &mut VM, statements: &Statements) -> Block {
    let mut c = Context::new(&mut vm);
    for ref s in statements {
        gen_statement(&mut c, s);
    }
    c.block.finalize();
    c.block;
}
