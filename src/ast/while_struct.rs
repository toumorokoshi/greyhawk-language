use super::{Statements, Expression};

#[derive(Clone, PartialEq, Debug)]
pub struct While {
    pub condition: Box<Expression>,
    pub block: Statements
}
