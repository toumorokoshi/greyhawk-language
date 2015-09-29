use super::ops;
use std::rc::Rc;

pub struct VMFunction {
    pub register_count: usize,
    pub ops: Vec<ops::Op>
}

pub enum Function {
    NativeFunction{function: fn() -> i32},
    VMFunction(VMFunction)
}
