mod expression;

use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;

pub type ExprResult = Result<Box<codegen::Expression>, &'static str>;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<codegen::Expression>> {
    let mut token_iter = tokens.iter().peekable();
    let mut expressions: Vec<Box<codegen::expressions::Expression>> = Vec::new();
    let expr = expression::parse_expression(&mut tokens.iter().peekable());
    match expr {
        Ok(e) => expressions.push(e),
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return expressions;
}
