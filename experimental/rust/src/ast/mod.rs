use lexer::token::TokenType;

pub enum Expression {
    ConstInt{value: i32},
    ConstFloat{value: f32},
    BinOp{op: TokenType, left: Box<Expression>, right: Box<Expression>},
    Call{name: String, arg: Box<Expression>}
}

pub enum Statement {
    FunctionDecl{name: String, statements: Vec<Statement>},
    Return(Expression),
    Expr(Expression)
}
