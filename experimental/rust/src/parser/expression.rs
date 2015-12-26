use ast;
use ast::Expression;
use std::rc::Rc;
use std::slice::Iter; use std::iter::Peekable;
use lexer;
use lexer::token::TokenType;
use codegen;
use super::{Parser, PResult, PMatchResult, ErrorMessage, to_match_result};
use super::expect;

pub type ExprResult = PResult<Expression>;

pub fn parse_expression(parser: &mut Parser) -> PMatchResult<Expression> {
    let mut expr = try_match!(parse_base_value(parser));
    if let None = parser.peek() { return PMatchResult::Ok(expr); }
    loop {
        match try_token!(parser.peek(), "parse_expression".to_string()).clone().typ {
            TokenType::Plus | TokenType::Sub | TokenType::Mul | TokenType::Div => {
                expr = try_match!(parse_binary_operation(parser, expr));
            },
            _ => {return PMatchResult::Ok(expr);}
        }
        if let None = parser.peek() {
            return PMatchResult::Ok(expr);
        }
    }
}

pub fn parse_binary_operation(parser: &mut Parser, left: Expression) -> PMatchResult<Expression> {
    let token_type = match parser.peek() {
        Some(ref t) => match t.typ {
            TokenType::Plus => TokenType::Plus,
            TokenType::Sub => TokenType::Sub,
            TokenType::Mul => TokenType::Mul,
            TokenType::Div => TokenType::Div,
            _ => {return PMatchResult::Ok(left)}
        },
        None => {return PMatchResult::NoMatch},
    };
    parser.next(); // consume token if we got this far.
    let right = try_match!(parse_base_value(parser));
    return PMatchResult::Ok(
        Expression::BinOp(ast::BinOp{op: token_type, left: Box::new(left), right: Box::new(right)})
    );
}

pub fn parse_base_value(parser: &mut Parser) -> PMatchResult<Expression> {
    match try_token!(parser.next(), "parse_base_value".to_string()).typ {
        TokenType::Int(i) => Ok(ast::Expression::ConstInt{value: i}),
        TokenType::Float(f) => Ok(ast::Expression::ConstFloat{value: f}),
        TokenType::Symbol(ref s) => to_match_result(parse_call(s.clone(), parser)),
        _ => PMatchResult::NoMatch
    }
}


pub fn parse_call(name: String, parser: &mut Parser) -> ExprResult {
    try!(expect::expect(parser, TokenType::ParenL));
    let expr: Expression = match parse_expression(parser) {
        PMatchResult::Ok(e) => e,
        PMatchResult::NoMatch => return PMatchResult::Err(
            vec![ErrorMessage{message: "no match for expression", token: None}]
        ),
        PMatchResult::Err(err) => return PMatchResult::Err(err)
    };
    try!(expect::expect(parser, TokenType::ParenR));
    Ok(Expression::Call{name: name, arg: Box::new(expr)});
}
