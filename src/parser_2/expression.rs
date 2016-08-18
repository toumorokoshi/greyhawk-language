use ast;
use ast::Expression;
use lexer::token::TokenType;
use super::{Parser, PResult, ErrorMessage};
use super::expect;

pub fn parse_expression(parser: &mut Parser) -> PResult<Option<Expression>> {
    let mut expr = try_error_option!(parse_base_value(parser));
    if let None = parser.peek() { return Ok(Some(expr)); }
    loop {
        match try_token!(parser.peek(), "parse_expression".to_string()).clone().typ {
            TokenType::Plus | TokenType::Sub | TokenType::Mul | TokenType::Div => {
                match parse_binary_operation(parser, expr.clone()) {
                    Ok(maybe) => match maybe {
                        Some(next_expr) => {expr = next_expr},
                        None => {return Ok(Some(expr));},
                    },
                    Err(err) => {return Err(err);},
                }
            },
            _ => {return Ok(Some(expr));}
        }
        if let None = parser.peek() {
            return Ok(Some(expr));
        }
    }
}

pub fn parse_binary_operation(parser: &mut Parser, left: Expression) -> PResult<Option<Expression>> {
    let token_type = match parser.peek() {
        Some(ref t) => match t.typ {
            TokenType::Plus => TokenType::Plus,
            TokenType::Sub => TokenType::Sub,
            TokenType::Mul => TokenType::Mul,
            TokenType::Div => TokenType::Div,
            _ => {return Ok(None);}
        },
        None => {return Ok(None);},
    };
    parser.next(); // consume token if we got this far.
    let right = try_error_option!(parse_base_value(parser));
    return Ok(Some(
        Expression::BinOp(ast::BinOp{op: token_type, left: Box::new(left), right: Box::new(right)})
    ));
}

pub fn parse_base_value(parser: &mut Parser) -> PResult<Option<Expression>> {
    match try_token!(parser.next(), "parse_base_value".to_string()).typ {
        TokenType::Int(i) => Ok(Some(ast::Expression::ConstInt{value: i})),
        TokenType::Float(f) => Ok(Some(ast::Expression::ConstFloat{value: f})),
        TokenType::String(ref s) => Ok(Some(ast::Expression::ConstString{value: s.clone()})),
        TokenType::Symbol(ref s) => match parse_call(s.clone(), parser) {
            Ok(result) => Ok(Some(result)),
            Err(err) => Err(err)
        },
        _ => Ok(None),
    }
}


pub fn parse_call(name: String, parser: &mut Parser) -> PResult<Expression> {
    try!(expect::expect(parser, TokenType::ParenL));
    let expr = try_option!(
        try!(parse_expression(parser)),
        vec![ErrorMessage{message: "no match for expression".to_string(), token: None}]
    );
    try!(expect::expect(parser, TokenType::ParenR));
    let expr_boxed: Box<Expression> = Box::new(expr);
    Ok(Expression::Call{name: name, arg: expr_boxed})
}
