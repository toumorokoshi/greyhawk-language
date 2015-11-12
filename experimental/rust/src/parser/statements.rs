use codegen;
use lexer;
use super::expression;
use lexer::token::TokenType;
use std::iter::Peekable;
use std::slice::Iter;
use super::expect;
use super::expect_next;
pub type StatResult = Result<codegen::Statement, &'static str>;
use super::Parser;

pub fn parse_statement(parser: &mut Parser) -> StatResult {
    match parser.cur_typ() {
        TokenType::Type(name) => parse_function_declaration(parser),
        _ => match expression::parse_expression(parser) {
            Ok(expr) => Ok(codegen::Statement::Expression(expr)),
            Err(err) => Err(err)
        }
    }
}

pub fn parse_function_declaration(parser: &mut Parser) -> StatResult {
    let typ = try!(expect::typ(parser));
    let symbol = try!(expect::symbol(parser));
    try!(expect::expect(parser, TokenType::ParenL));
    try!(expect::expect(parser, TokenType::ParenR));
    try!(expect::expect(parser, TokenType::Colon));
    let statements = Vec::new();
    while (
    Err("expected type for function declaration.")
}
