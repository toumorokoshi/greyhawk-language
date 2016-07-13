use super::{Statements, Expression};


#[derive(Clone, PartialEq, Debug)]
pub struct Condition {
    pub true_block: Statements,
    pub false_block: Statements,
    pub expression: Box<Expression>
}
