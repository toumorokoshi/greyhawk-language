use lexer::token::TokenType;
pub mod yaml;
mod assignment;
mod declaration;
pub use self::assignment::Assignment;
pub use self::declaration::Declaration;

#[derive(Clone, PartialEq, Debug)]
pub struct BinOp {
    pub op: TokenType,
    pub left: Box<Expression>,
    pub right: Box<Expression>
}

#[derive(Clone, PartialEq, Debug)]
pub struct FunctionDecl {
    pub name: String,
    pub statements: Vec<Box<Statement>>,
    pub typ: String,
}

#[derive(Clone, PartialEq, Debug)]
pub enum Expression {
    ConstInt{value: i64},
    ConstFloat{value: f64},
    ConstString{value: String},
    BinOp(BinOp),
    Symbol(String),
    Call{name: String, arg: Box<Expression>}
}

#[derive(Clone, Debug, PartialEq)]
pub enum Statement {
    FunctionDecl(FunctionDecl),
    Return(Expression),
    Expr(Expression),
    Assignment(Assignment),
    Declaration(Declaration),
}

pub type Statements = Vec<Box<Statement>>;
