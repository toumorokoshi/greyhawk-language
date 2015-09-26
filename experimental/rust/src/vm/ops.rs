pub enum Op {
    LoadInt { register: usize, constant: i32},
    AddConstantInt { register: usize, constant: i32},
    AddInt{lhs: usize, rhs: usize},
}
