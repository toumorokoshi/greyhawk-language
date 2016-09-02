use ast::{Statements};
use vm::{VM, Op, Scope, VMFunction};
use super::gen_statement;

pub fn gen_block(vm: &mut VM, statements: &Statements) -> Block {
    let mut b = Block::new();
    for ref s in statements {
        gen_statement(vm, s, &mut b);
    }
    let mut b = Block {ops: ops, scope: scope};
    b.finalize();
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
    pub fn new(&mut self, Option<Arc<) {
        return Block{
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
