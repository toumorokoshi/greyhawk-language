use codegen;
use lexer::token::TokenType;

type ExprResult = Result<&'static str, Box<Expression>>;

pub fn parse_expression(tokens: &mut Peekable<lexer::Token>) -> ExprResult {
    return parse_binary_operation(tokens);
}

pub fn parse_binary_operation(tokens: &mut Peekable<lexer::Token>) -> ExprResult {
    let mut left = parse_value();
    // Box::new(codegen::IntExpression{value: 10}));
}

pub fn parse_value(tokens: &mut Peekable<lexer::Token>) -> ExprResult {
}

pub fn parse_base_value(tokens: &mut Peekable<lexer::Token>) -> ExprResult {
    let next = token.peek();
    return match next.tok {
        TokenType::Int(i) => return Ok(Box::new(codegen::IntExpression{value: i})),
    }
}
