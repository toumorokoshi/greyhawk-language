use super::register;
use super::ops;

pub enum Function {
    NativeFunction{function: fn() -> register::Register},
    VMFunction{ops: &'static [ops::Op]},
}
