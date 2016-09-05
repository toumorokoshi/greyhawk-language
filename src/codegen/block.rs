use ast::{Statements};
use vm::{VM, Op, Scope, VMFunction};
use super::{
    Context,
    gen_statement,
    CGResult
};
use std::collections::HashMap;

pub fn gen_block(vm: &mut VM, statements: &Statements) -> CGResult<Block> {
    let mut c = Context::new(vm);
    for ref s in statements {
        try!(gen_statement(&mut c, s));
    }
    c.block.finalize();
    Ok(c.block)
}

pub struct Block {
    pub ops: Vec<Op>,
    pub scope: Scope,
    // functions have to be compiled at the end,
    // to ensure variables in the outer scope
    // are resolved.
    pub functions: HashMap<String, VMFunction>,
}

impl Block {
    pub fn new() -> Block {
        Block {
            ops: vec![],
            scope: Scope::new(None),
            functions: HashMap::new()
        }
    }

    /// finalize the construction of
    /// the functions that were collected
    /// by the block.
    pub fn build_functions(&mut self) {
    }

    pub fn finalize(&mut self) {
        self.build_functions();
    }
}
