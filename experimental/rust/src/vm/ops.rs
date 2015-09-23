pub enum Op {
    AddConstantInt { register: usize, constant: i32},
    AddInt{lhs: usize, rhs: usize},
    ExecuteFunction { name: &'static str },
}
