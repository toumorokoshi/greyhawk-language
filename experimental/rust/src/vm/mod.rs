use std::collections::HashMap;

pub mod module;
pub mod function;
pub mod register;

pub struct VM {
    pub memory: i32
}

pub enum Op {
    AddInt { lhs: i32, rhs: i32 }
}

impl VM {
    pub fn execute_instructions(&self, ops: &[Op]) {
        for op in ops {
            match op {
                &Op::AddInt{lhs, rhs} => println!("{}", lhs + rhs),
            };
        }
    }
}
