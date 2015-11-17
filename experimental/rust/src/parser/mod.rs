macro_rules! try_option {
    ($expr:expr) => (match $expr {
        Some(val) => val,
        None => return Err("expected another token")
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

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<ast::Statement> {
    let mut parser = tokens.iter().peekable();
    let mut statements: Vec<ast::Statement> = Vec::new();
    let stat = statements::parse_statement(&mut parser);
    match stat {
        Ok(s) => statements.push(s),
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return statements;
}

fn expect_next(typ: TokenType, message: &'static str, tokens: &mut Peekable<Iter<lexer::Token>>) -> Result<(), &'static str> {
    match tokens.next() {
        Some(t) => {
            if typ == t.typ {
                Ok(())
            } else {
                Err(message)
            }
        },
        None => Err("unable to find next token")
    }
}

pub type Parser<'a> = Peekable<Iter<'a, lexer::Token>>;
