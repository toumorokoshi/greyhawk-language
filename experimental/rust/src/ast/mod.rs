use lexer::token::TokenType;
pub mod yaml;

pub struct BinOp {
    pub op: TokenType,
    pub left: Box<Expression>,
    pub right: Box<Expression>
}

pub enum Expression {
    ConstInt{value: i32},
    ConstFloat{value: f32},
    BinOp(BinOp),
    Call{name: String, arg: Box<Expression>}
}

pub enum Statement {
    FunctionDecl{name: String, statements: Vec<Statement>},
    Return(Expression),
    Expr(Expression)
}
