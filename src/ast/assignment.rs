use super::Expression;
use std::fmt;

#[derive(Clone, PartialEq)]
pub struct Assignment {
    pub name: String,
    pub expression: Box<Expression>,
}

impl fmt::Debug for Assignment {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&(format!("assignment to {}", self.name)))
    }
}
