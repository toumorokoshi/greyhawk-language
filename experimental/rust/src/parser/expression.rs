use std::rc::Rc;
use std::slice::Iter;
use std::iter::Peekable;
use lexer;
use lexer::token::TokenType;
use codegen;
use super::expect_next;
use super::Parser;
use super::expect;

pub type ExprResult = Result<Box<codegen::Expression>, &'static str>;

pub fn parse_expression(parser: &mut Parser) -> ExprResult {
    parse_binary_operation(parser)
}

pub fn parse_binary_operation(parser: &mut Parser) -> ExprResult {
    let left = parse_base_value(parser);
    let token_type = match parser.next().typ {
        TokenType::Plus => TokenType::Plus,
        TokenType::Sub => TokenType::Sub,
        TokenType::Mul => TokenType::Mul,
        TokenType::Div => TokenType::Div,
        _ => return left
    };
    let right = parse_base_value(parser);
    return match left {
        Ok(l) => match right {
            Ok(r) => Ok(Box::new(codegen::BinOpExpression{op: token_type, left: l, right: r})),
            Err(e) => Err(e),
        },
        Err(e) => Err(e),
    };
}

pub fn parse_base_value(parser: &mut Parser) -> ExprResult {
    match parser.next().typ {
        TokenType::Int(i) => Ok(Box::new(codegen::IntExpression{value: i})),
        TokenType::Float(f) => Ok(Box::new(codegen::FloatExpression{value: f})),
        TokenType::Symbol(ref s) => parse_call(s.clone(), parser),
        _ => Err("unable to find basic type."),
    }
}


pub fn parse_call(name: String, parser: &mut Parser) -> ExprResult {
    try!(expect::expect(parser, TokenType::ParenL));
    let expr_result = parse_expression(parser);
    try!(expect::expect(parser, TokenType::ParenR));
    match expr_result {
        Ok(expr) => Ok(Box::new(codegen::CallExpression {
            name: name,
            arg: expr
        })),
        Err(m) => Err(m)
    }
}
