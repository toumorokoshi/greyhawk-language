use ast::{Statements};
use vm::{VM, Op, Scope};
use super::gen_statement;

pub struct Block {
    pub ops: Vec<Op>,
    pub scope: Scope
}

pub fn gen_block(vm: &mut VM, statements: &Statements) -> Block {
    let mut ops: Vec<Op> = Vec::new();
    let mut scope = Scope::new();
    for ref s in statements {
        gen_statement(vm, s, &mut scope, &mut ops);
    }
    return Block {ops: ops, scope: scope};
}
