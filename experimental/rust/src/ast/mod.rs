use lexer::token::TokenType;
pub mod yaml;

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

pub enum Expression {
    ConstInt{value: i32},
    ConstFloat{value: f32},
    BinOp(BinOp),
    Call{name: String, arg: Box<Expression>}
}

pub enum Statement {
    FunctionDecl(FunctionDecl),
    Return(Expression),
    Expr(Expression)
}

pub type Statements = Vec<Box<Statement>>;
