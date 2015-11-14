use lexer::token::TokenType;

pub enum Statement {
    FunctionDecl{name: String, statements: Vec<Statement>},
    Return(Expression),
    Expr(Expression)
}

pub enum Expression {
    ConstInt{value: i32},
    ConstFloat{value: f32},
    BinOp{op: TokenType, left: Expression, right: Expression},
    Call{name: String, arg: Expression}
}
