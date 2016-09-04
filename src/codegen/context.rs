use super::Block;
use vm::VM;

/// the context that the
/// code generator is currently executing in.
pub struct Context<'a> {
    pub block: Block,
    pub vm: &'a mut VM
}

impl<'a> Context<'a> {
    pub fn new(vm: &'a mut VM) -> Context {
        Context{block: Block::new(), vm: vm}
    }
}
