use super::ops;

pub enum Function {
    NativeFunction{function: fn() -> i32},
    VMFunction{
        register_count: usize,
        ops: Box<[ops::Op]>
    },
}
