mod expression;
mod statements;

use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;
use self::statements::StatResult;
use self::expression::ExprResult;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<codegen::Statement>> {
    let mut token_iter = tokens.iter().peekable();
    let mut statements: Vec<Box<codegen::statements::Statement>> = Vec::new();
    let expr = statements::parse_statement(&mut tokens.iter().peekable());
    match expr {
        Ok(e) => statements.push(e),
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return statements;
}
