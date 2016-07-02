use lexer::token::TokenType;
pub mod yaml;
mod assignment;
pub use self::assignment::Assignment;

#[derive(Clone, PartialEq)]
pub struct BinOp {
    pub op: TokenType,
    pub left: Box<Expression>,
    pub right: Box<Expression>
}

pub struct FunctionDecl {
    pub name: String,
    pub statements: Vec<Box<Statement>>,
    pub typ: String,
}

#[derive(Clone, PartialEq)]
pub enum Expression {
    ConstInt{value: i64},
    ConstFloat{value: f64},
    ConstString{value: String},
    BinOp(BinOp),
    Symbol(String),
    Call{name: String, arg: Box<Expression>}
}

pub enum Statement {
    FunctionDecl(FunctionDecl),
    Return(Expression),
    Expr(Expression),
    Assignment(Assignment)
}

pub type Statements = Vec<Box<Statement>>;
