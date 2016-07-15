use ast::{Statements};
use vm::{Op, Scope};
use super::gen_statement;

pub struct Block {
    pub ops: Vec<Op>,
    pub scope: Scope
}

pub fn gen_block(statements: &Statements) -> Block {
    let mut ops: Vec<Op> = Vec::new();
    let mut scope = Scope::new();
    for s in statements {
        gen_statement(s, &mut scope, &mut ops);
    }
    return Block {ops: ops, scope: scope};
}
