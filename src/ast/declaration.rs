use super::Expression;
use std::fmt;

#[derive(Clone, PartialEq)]
pub struct Declaration {
    pub name: String,
    pub expression: Box<Expression>,
}

impl fmt::Debug for Declaration {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&(format!("declaration to {}", self.name)))
    }
}
