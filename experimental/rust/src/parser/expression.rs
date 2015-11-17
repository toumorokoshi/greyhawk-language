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
    let mut expr = parse_base_value(parser);
    loop {
        match try_option!(parser.peek()).typ {
            TokenType::Plus
                | TokenType::Sub
                | TokenType::Mul
                | TokenType::Div => {
                    match expr {
                        Ok(e) => {
                            expr = parse_binary_operation(parser, e);
                        },
                        Err(err) => {return Err(err);},
                    }
                }
            _ => { return expr; }
        }
        if let None = parser.peek() {
            return expr;
        }
    }
}

pub fn parse_binary_operation(parser: &mut Parser, left: ast::Expression) -> ExprResult {
    let token_type = match parser.peek() {
        Some(ref t) => match t.typ {
            TokenType::Plus => TokenType::Plus,
            TokenType::Sub => TokenType::Sub,
            TokenType::Mul => TokenType::Mul,
            TokenType::Div => TokenType::Div,
            _ => return Ok(left)
        },
        None => return Err("unable to find a token.")
    };
    let right = parse_base_value(parser);
    match right {
        Ok(r) => Ok(ast::Expression::BinOp{
            op: token_type, left: Box::new(left), right: Box::new(r)
        }),
        Err(e) => Err(e),
    }
}

pub fn parse_base_value(parser: &mut Parser) -> ExprResult {
    match try_option!(parser.peek()).typ {
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
