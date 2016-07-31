use super::{Expression, ExpressionList};

#[derive(Clone, PartialEq, Debug)]
pub struct ArrayCreate {
    pub size: Box<Expression>,
    pub typ: String,
    pub values: ExpressionList,
}

#[derive(Clone, PartialEq, Debug)]
pub struct IndexGet {
    pub source: Box<Expression>,
    pub index: Box<Expression>,
}

#[derive(Clone, PartialEq, Debug)]
pub struct IndexSet {
    pub target: Box<Expression>,
    pub value: Box<Expression>,
    pub index: Box<Expression>,
}
