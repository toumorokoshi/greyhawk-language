use super::ops;

pub enum Function {
    NativeFunction{function: fn() -> i32},
    VMFunction{
        register_count: u8,
        ops: &'static [ops::Op]
    },
}
