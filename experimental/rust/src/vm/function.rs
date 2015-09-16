use super::register;
use super::value;
use super::ops;

pub enum Function {
    NativeFunction{function: fn() -> value::Value},
    VMFunction{ops: &'static [ops::Op]},
}
