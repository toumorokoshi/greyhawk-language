pub enum Op {
    AddInt { lhs: i32, rhs: i32 },
    ExecuteFunction { name: &'static str },
}
