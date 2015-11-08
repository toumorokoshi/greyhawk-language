use codegen;
use lexer;
use super::expression;
use lexer::token::TokenType;
use std::iter::Peekable;
use std::slice::Iter;
pub type StatResult = Result<codegen::Statement, &'static str>;

pub fn parse_statement(tokens: &mut Peekable<Iter<lexer::Token>>) -> StatResult {
    let mut next_token: Option<lexer::Token> = None;
    if let Some(next) = tokens.peek() { next_token = Some((**next).clone()); }

    match next_token {
        Some(t) => {
            match t.typ {
                TokenType::Type(name) => parse_function_declaration(tokens),
                _ => match expression::parse_expression(tokens) {
                    Ok(expr) => Ok(codegen::Statement::Expression(expr)),
                    Err(err) => Err(err)
                }
            }
        },
        None => Err("no token next.")
    }
}

pub fn parse_function_declaration(tokens: &mut Peekable<Iter<lexer::Token>>) -> StatResult {
    return Err("test");
}
