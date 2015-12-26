macro_rules! try_option {
    ($expr:expr, $err:expr) => (match $expr {
        Some(val) => val.clone(),
        None => return Err($err)
    })
}

macro_rules! try_token {
    ($expr:expr, $err:expr) => match $expr {
        Some(val) => val.clone(),
        None => return Err(vec![ErrorMessage{message: $err, token: None}])
    })
}

macro_rules! try_compound {
    ($expr:expr, $err:expr) => (match $expr {
        Ok(val) => val.clone(),
        Err(message) => return Err(format!("{}, {}", $err, message))
    })
}

macro_rules! try_match {
   ($expr: expr) => (match $expr {
       PMatchResult::Ok(val) => val.clone(),
       PMatchResult::NoMatch => return PMatchResult::NoMatch,
       PMatchResult::Err(err) => return PMatchResult::Err(err)
   })
}

pub type Parser<'a> = Peekable<Iter<'a, lexer::Token>>;

pub struct ErrorMessage {
    message: String,
    token: Option<lexer::Token>
}

pub type PResult<T> = Result<T, Vec<ErrorMessage>>;

impl PResult<T> {
    fn to_match_result(self) {
        match self {
            Ok(result) => PMatchResult::Ok(result),
            Err(err) => PMatchResult::Err(err),
        }
    }
}

pub enum PMatchResult<T> = {
    Ok(T),
    NoMatch,
    Err(Vec<ErrorMessage>)
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

pub fn parse(tokens: &Vec<lexer::Token>) -> Result<ast::Statements, String> {
    let mut parser = tokens.iter().peekable();
    match parse_statements(&mut parser) {
        Ok(statements) => statements,
        Err(errors) => {
            let message_strings: Vec<String> = vec::new();
            for (err in errors) {
                match err.token {
                    Some(t) => {
                        message_strings.push(format!("line {}, {}: {}",
                                                     err.token.line_num,
                                                     err.token.typ,
                                                     message));
                    },
                    None => {
                        message_strings.push(format!("{}", message));
                    }
                }
            }
            Err(message_strings.join("\n"))
        }
    }
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
                match statements::parse_statement(parser) {
                    Ok(s) => statements.push(Box::new(s)),
                    NoMatch => return Ok(statements),
                    Err(err) => return Err(err)
                }
            },
            None => {return Ok(statements);},
        }
    }
}
