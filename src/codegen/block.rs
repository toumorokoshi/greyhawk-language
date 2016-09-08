use ast::{Statements, FunctionDecl};
use vm::{VM, Op, Scope, VMFunction};
use super::{
    Context,
    gen_statement,
    gen_function
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
    pub functions: HashMap<String, FunctionDecl>,
}

impl Block {
    pub fn new(parent_scope: Option<Arc<Scope>>) -> Block {
        Block {
            ops: vec![],
            scope: Scope::new(parent_scope),
            functions: HashMap::new()
        }
    }

    /// finalize the construction of
    /// the functions that were collected
    /// by the block.
    pub fn build_functions(&mut self) {
        for name, func_decl in self.functions {
        }
    }

    pub fn finalize(&mut self) {
        self.build_functions();
    }
}
