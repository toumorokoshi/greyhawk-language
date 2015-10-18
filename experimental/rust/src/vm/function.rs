use super::ops;
use super::scope;
use std::rc::Rc;

pub struct VMFunction {
    pub scope: scope::Scope,
    pub ops: Vec<ops::Op>
}

pub enum Function {
    NativeFunction{
        function: fn() -> i32,
        typ: super::types::TypeRef
    },
    VMFunction(VMFunction)
}
