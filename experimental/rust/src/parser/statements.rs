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
use super::{PResult, PMatchResult, ErrorMessage, to_match_result};

pub fn parse_statement(parser: &mut Parser) -> PMatchResult<Statement> {
    match try_token!(parser.peek(), "parse_statement".to_string()).typ {
        TokenType::Type(ref name) => parse_function_declaration(parser),
        TokenType::Return => to_match_result(parse_return(parser)),
        _ => match expression::parse_expression(parser) {
            Ok(expr) => PMatchResult::Ok(ast::Statement::Expr(expr)),
            Err(err) => PMatchResult::Err(err)
        }
    }
}

pub fn parse_return(parser: &mut Parser) -> PResult<Statement> {
    try!(expect::expect(parser, TokenType::Return));
    match expression::parse_expression(parser) {
        Ok(expr) => Ok(ast::Statement::Return(expr)),
        Err(msg) => Err(msg)
    }
}

pub fn parse_function_declaration(parser: &mut Parser) -> PResult<Statement> {
    let typ = try_compound!(expect::typ(parser), "in function declaration");
    let symbol = try_compound!(expect::symbol(parser), "in function declaration");
    try_compound!(expect::expect(parser, TokenType::ParenL), "in function declaration");
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
