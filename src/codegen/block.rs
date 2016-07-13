use ast::{Statements};
use vm::{Op, Scope};

pub struct Block {
    pub ops: Vec<Op>,
    pub scope: Scope
}

pub fn gen_block(statements: &Statements) -> Block {
    let mut ops: Vec<Op> = Vec::new();
}
