use codegen;
use lexer;
use super::expression;
use lexer::token::TokenType;
use std::iter::Peekable;
use std::slice::Iter;
pub type StatResult = Result<Box<codegen::Statement>, &'static str>;

pub fn parse_statement(tokens: &mut Peekable<Iter<lexer::Token>>) -> StatResult {
    if let Some(ref next) = tokens.peek() {
        let next_clone = next.clone();
        match next_clone.typ {
            TokenType::Type(name) => parse_function_declaration(&mut tokens),
            _ => match expression::parse_expression(&mut tokens) {
                Ok(expr) => Ok(expr.as_statement()),
                Err(err) => Err(err)
            }
        }
    } else {
        Err("test")
    }
}

pub fn parse_function_declaration(tokens: &mut Peekable<Iter<lexer::Token>>) -> StatResult {
    return Err("test");
}
