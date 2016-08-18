macro_rules! try_option {
    ($expr:expr, $err:expr) => (match $expr {
        Some(val) => val,
        None => return Err($err)
    })
}

macro_rules! try_error_option {
    ($expr:expr) => (match $expr {
        Ok(maybe) => match maybe {
            Some(option) => option,
            None => return Ok(None),
        },
        Err(err) => {return Err(err);}
    })
}

macro_rules! try_token {
    ($expr:expr, $err:expr) => (match $expr {
        Some(val) => val.clone(),
        None => return Err(vec![ErrorMessage{message: $err, token: None}])
    })
}

pub type Parser<'a> = Peekable<Iter<'a, lexer::Token>>;

pub struct ErrorMessage {
    message: String,
    token: Option<lexer::Token>
}

pub type PResult<T> = Result<T, Vec<ErrorMessage>>;

mod expression;
mod statements;
mod expect;

use ast;
use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;

pub fn parse(tokens: &Vec<lexer::Token>) -> Result<ast::Statements, String> {
    let mut parser = tokens.iter().peekable();
    match parse_statements(&mut parser) {
        Ok(statements) => Ok(statements),
        Err(errors) => {
            let mut message_strings: Vec<String> = Vec::new();
            for err in errors {
                match err.token {
                    Some(t) => {
                        message_strings.push(format!("line {}, {}: {}",
                                                     t.line_num,
                                                     t.typ,
                                                     err.message));
                    },
                    None => {
                        message_strings.push(format!("{}", err.message));
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
        let next: Option<lexer::Token> = {
            match parser.peek() {
                Some(ref t) => Some((**t).clone()),
                None => None,
            }
        };
        match next {
            Some(t) => {
                match statements::parse_statement(parser) {
                    Ok(maybe_statement) => match maybe_statement {
                        Some(statement) => {statements.push(Box::new(statement));}
                        None => {return Ok(statements);}
                    },
                    Err(err) => return Err(err)
                }
            },
            None => {return Ok(statements);},
        }
    }
}
