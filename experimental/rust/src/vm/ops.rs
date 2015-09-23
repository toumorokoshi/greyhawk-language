pub enum Op {
    AddConstantInt { register: usize, constant: i32},
    AddInt{lhs_register: usize, rhs_register: u8},
    ExecuteFunction { name: &'static str },
}
