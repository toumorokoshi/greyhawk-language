use super::function;

pub enum Op {
    Call{func: function::Function, target: usize},
    FloatAdd{lhs: usize, rhs: usize, target: usize},
    FloatSub{lhs: usize, rhs: usize, target: usize},
    FloatMul{lhs: usize, rhs: usize, target: usize},
    FloatDiv{lhs: usize, rhs: usize, target: usize},
    FloatLoad{register: usize, constant: f32},
    IntAdd{lhs: usize, rhs: usize, target: usize},
    IntSub{lhs: usize, rhs: usize, target: usize},
    IntMul{lhs: usize, rhs: usize, target: usize},
    IntDiv{lhs: usize, rhs: usize, target: usize},
    IntLoad{register: usize, constant: i32},
    Return{register: usize},
}
