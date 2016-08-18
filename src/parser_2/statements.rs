use ast;
use ast::Statement;
use lexer;
use super::parse_statements;
use super::expression;
use lexer::token::TokenType;
use std::iter::Peekable;
use std::slice::Iter;
use codegen;
use super::expect;
use super::Parser;
use super::{PResult, ErrorMessage};

pub fn parse_statement(parser: &mut Parser) -> PResult<Option<Statement>> {
    match try_token!(parser.peek(), "parse_statement".to_string()).typ {
        TokenType::Type(ref name) => match parse_function_declaration(parser) {
            Ok(r) => Ok(Some(r)), Err(e) => Err(e)
        },
        TokenType::Return => match parse_return(parser) {
            Ok(r) => Ok(Some(r)), Err(e) => Err(e)
        },
        _ => match expression::parse_expression(parser) {
            Ok(maybe) => match maybe {
                Some(expr) => Ok(Some(Statement::Expr(expr))),
                None => Ok(None),
            },
            Err(err) => Err(err)
        }
    }
}

pub fn parse_return(parser: &mut Parser) -> PResult<Statement> {
    try!(expect::expect(parser, TokenType::Return));
    match expression::parse_expression(parser) {
        Ok(expr) => match expr {
            Some(expr) => Ok(Statement::Return(expr)),
            None => Err(vec![ErrorMessage{message: "unable to find expression for return".to_string(), token: None}])
        },
        Err(msg) => Err(msg)
    }
}

pub fn parse_function_declaration(parser: &mut Parser) -> PResult<Statement> {
    let typ = try!(expect::typ(parser));
    let symbol = try!(expect::symbol(parser));
    try!(expect::expect(parser, TokenType::ParenL));
    try!(expect::expect(parser, TokenType::ParenR));
    try!(expect::expect(parser, TokenType::Colon));
    try!(expect::expect(parser, TokenType::Indent));
    let inner_statements = try!(parse_statements(parser));
    try!(expect::expect(parser, TokenType::Unindent));
    return Ok(ast::Statement::FunctionDecl(ast::FunctionDecl{
        name: symbol,
        typ: typ,
        statements: inner_statements
    }));
}
