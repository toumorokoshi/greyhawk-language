use ast;
use lexer;
use super::parse_statements;
use super::expression;
use lexer::token::TokenType;
use std::iter::Peekable;
use std::slice::Iter;
use codegen;
use super::expect;
pub type StatResult = Result<ast::Statement, String>;
use super::Parser;

pub fn parse_statement(parser: &mut Parser) -> StatResult {
    match try_option!(parser.peek(), "parse_statement".to_string()).typ {
        TokenType::Type(ref name) => parse_function_declaration(parser),
        TokenType::Return => parse_return(parser),
        _ => match expression::parse_expression(parser) {
            Ok(expr) => Ok(ast::Statement::Expr(expr)),
            Err(err) => Err(err)
        }
    }
}

pub fn parse_return(parser: &mut Parser) -> StatResult {
    try!(expect::expect(parser, TokenType::Return));
    match expression::parse_expression(parser) {
        Ok(expr) => Ok(ast::Statement::Return(expr)),
        Err(msg) => Err(msg)
    }
}

pub fn parse_function_declaration(parser: &mut Parser) -> StatResult {
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
