use lexer::token::TokenType;
pub mod yaml;

#[derive(Clone)]
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

#[derive(Clone)]
pub enum Expression {
    ConstInt{value: i64},
    ConstFloat{value: f64},
    ConstString{value: String},
    BinOp(BinOp),
    Call{name: String, arg: Box<Expression>}
}

pub enum Statement {
    FunctionDecl(FunctionDecl),
    Return(Expression),
    Expr(Expression)
}

pub type Statements = Vec<Box<Statement>>;
