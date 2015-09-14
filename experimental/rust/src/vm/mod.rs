pub enum Register {
    Int(i32)
}

pub enum Op {
    AddInt { lhs: i32, rhs: i32 }
}

pub fn execute_instructions(ops: &[Op]) {
    for op in ops {
        match op {
            &Op::AddInt{lhs, rhs} => println!("{}", lhs + rhs),
        };
    }
}
