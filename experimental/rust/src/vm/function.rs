use super::register;

pub enum Function {
    NativeFunction{function: fn() -> register::Register},
}
