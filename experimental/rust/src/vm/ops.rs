pub enum Op {
    IntAdd{lhs: usize, rhs: usize, target: usize},
    IntSub{lhs: usize, rhs: usize, target: usize},
    IntMul{lhs: usize, rhs: usize, target: usize},
    IntDiv{lhs: usize, rhs: usize, target: usize},
    IntLoad{ register: usize, constant: i32},
    FloatAdd{lhs: usize, rhs: usize, target: usize},
    FloatSub{lhs: usize, rhs: usize, target: usize},
    FloatMul{lhs: usize, rhs: usize, target: usize},
    FloatDiv{lhs: usize, rhs: usize, target: usize},
    FloatLoad { register: usize, constant: f32},
    Return{register: usize},
}
