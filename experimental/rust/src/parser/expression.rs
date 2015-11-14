use ast;
use std::rc::Rc;
use std::slice::Iter; use std::iter::Peekable;
use lexer;
use lexer::token::TokenType;
use codegen;
use super::expect_next;
use super::Parser;
use super::expect;

pub type ExprResult = Result<ast::Expression, &'static str>;

pub fn parse_expression(parser: &mut Parser) -> ExprResult {
    let expr = parse_base_value(parser);
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
    match left {
        Ok(l) => match right {
            Ok(r) => Ok(ast::Expression::BinOp{
                op: token_type, left: Box::new(l), right: Box::new(r)
            }),
            Err(e) => Err(e),
        },
        Err(e) => Err(e),
    }
}

pub fn parse_base_value(parser: &mut Parser) -> ExprResult {
    match parser.next().typ {
        TokenType::Int(i) => Ok(ast::Expression::ConstInt{value: i}),
        TokenType::Float(f) => Ok(ast::Expression::ConstFloat{value: f}),
        TokenType::Symbol(ref s) => parse_call(s.clone(), parser),
        _ => Err("unable to find basic type."),
    }
}


pub fn parse_call(name: String, parser: &mut Parser) -> ExprResult {
    try!(expect::expect(parser, TokenType::ParenL));
    let expr_result = parse_expression(parser);
    try!(expect::expect(parser, TokenType::ParenR));
    match expr_result {
        Ok(expr) => Ok(ast::Expression::Call{name: name, arg: Box::new(expr)}),
        Err(m) => Err(m)
    }
}
