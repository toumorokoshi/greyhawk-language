pub mod yaml;
mod array;
mod assignment;
mod declaration;
mod binop;
mod condition;
mod while_struct;
pub use self::array::{ArrayCreate, ItemGet, ItemSet};
pub use self::assignment::Assignment;
pub use self::declaration::Declaration;
pub use self::binop::{BinOp, BinaryOperator};
pub use self::condition::Condition;
pub use self::while_struct::While;

#[derive(Clone, PartialEq, Debug)]
pub struct ArgumentDecl {
    pub name: String,
    pub typ: String
}

#[derive(Clone, PartialEq, Debug)]
pub struct FunctionDecl {
    pub name: String,
    pub arguments: Vec<ArgumentDecl>,
    pub statements: Vec<Box<Statement>>,
    pub typ: String,
}

#[derive(Clone, PartialEq, Debug)]
pub enum Expression {
    ArrayCreate(ArrayCreate),
    BinOp(BinOp),
    Call{name: String, args: ExpressionList},
    ConstInt{value: i64},
    ConstFloat{value: f64},
    ConstString{value: String},
    Condition(Condition),
    ItemGet(ItemGet),
    ItemSet(ItemSet),
    Symbol(String),
}

pub type ExpressionList = Vec<Box<Expression>>;

#[derive(Clone, Debug, PartialEq)]
pub enum Statement {
    Assignment(Assignment),
    Declaration(Declaration),
    Expr(Expression),
    FunctionDecl(FunctionDecl),
    Return(Expression),
    While(While),
}

pub type Statements = Vec<Box<Statement>>;
