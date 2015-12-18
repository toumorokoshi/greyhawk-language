macro_rules! try_option {
    ($expr:expr, $err:expr) => (match $expr {
        Some(val) => val.clone(),
        None => return Err($err)
    })
}

mod expression;
mod statements;
mod expect;

use ast;
use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;
use self::statements::StatResult;
use self::expression::ExprResult;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<ast::Statement>> {
    let mut parser = tokens.iter().peekable();
    let mut statements: Vec<Box<ast::Statement>> = Vec::new();
    let stat = statements::parse_statement(&mut parser);
    match stat {
        Ok(s) => statements.push(Box::new(s)),
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return statements;
}

pub type Parser<'a> = Peekable<Iter<'a, lexer::Token>>;
