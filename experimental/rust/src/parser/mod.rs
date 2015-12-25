macro_rules! try_option {
    ($expr:expr, $err:expr) => (match $expr {
        Some(val) => val.clone(),
        None => return Err($err)
    })
}

macro_rules! try_compound {
    ($expr:expr, $err:expr) => (match $expr {
        Ok(val) => val.clone(),
        Err(message) => return Err(format!("{}, {}", $err, message))
    })
}

pub type Parser<'a> = Peekable<Iter<'a, lexer::Token>>;
pub type PResult<T> = Result<T, String>;

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

pub fn parse(tokens: &Vec<lexer::Token>) -> PResult<ast::Statements> {
    let mut parser = tokens.iter().peekable();
    parse_statements(&mut parser)
}

pub fn parse_statements(parser: &mut Parser) -> PResult<ast::Statements> {
    let mut statements: ast::Statements = Vec::new();
    loop {
        let mut next: Option<lexer::Token> = {
            match parser.peek() {
                Some(ref t) => Some((**t).clone()),
                None => None,
            }
        };
        match next {
            Some(t) => {
                let stat = statements::parse_statement(parser);
                match stat {
                    Ok(s) => statements.push(Box::new(s)),
                    Err(err) => {
                        return Err(format!("unable to parse! line {}, {}\n", t.line_num, err));
                    },
                }
            },
            None => {break;},
        }
    }
    return Ok(statements);
}
