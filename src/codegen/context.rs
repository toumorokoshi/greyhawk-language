use super::Block;

/// the context that the
/// code generator is currently executing in.
pub struct Context {
    pub block: Block,
    pub vm: &mut VM
}

impl Context {
    pub fn new(vm: &mut VM) -> Context {
        block: Block::new(),
        vm: vm
    }
}
