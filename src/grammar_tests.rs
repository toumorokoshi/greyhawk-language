use super::peg_grammar::{
    binop,
    constant_string,
    declare_token,
    declare,
    expression,
    integer,
    statement,
    statement_list,
    symbol
};
use super::ast;
use lexer::token::{Token, TokenType};

#[test]
fn test_symbol() {
    assert_eq!(symbol("foo"), Ok(String::from("foo")));
}

#[test]
fn test_constant_string() {
    assert_eq!(
        constant_string("\"this is a string\""),
        Ok(String::from("this is a string"))
    );
}

#[test]
fn test_expression_symbol() {
    match expression("foo") {
        Ok(e) => match e {
            ast::Expression::Symbol(s) => {
                assert_eq!(s, String::from("foo"));
            },
            _ => {assert!(false)}
        },
        _ => {assert!(false)},
    }
}

#[test]
fn test_declare_token() {
    assert_eq!(declare_token(":="), Ok(Token{typ: TokenType::Assign,  line_num: 1}));
    assert!(declare_token("!=").is_err());
}

#[test]
fn test_integer() {
    assert_eq!(integer("12345"), Ok(12345));
}

#[test]
fn test_addition() {
    assert_eq!(binop("1 + 1"), Ok(
        ast::BinOp{
            left: Box::new(ast::Expression::ConstInt{value: 1}),
            right: Box::new(ast::Expression::ConstInt{value: 1}),
            op: TokenType::Plus
        }
    ));
}

#[test]
fn test_declare() {
    assert_eq!(declare("x := y"), Ok(
        ast::Declaration{name: String::from("x"),
                         expression: Box::new(ast::Expression::Symbol(String::from("y")))}
    ));
}

#[test]
fn test_statement() {
    assert_eq!(statement("x"),
               Ok(ast::Statement::Expr(
                   ast::Expression::Symbol(String::from("x")))
    ));
}

#[test]
fn test_statements() {
    assert_eq!(statement_list("x\nz"), Ok(vec![
        Box::new(ast::Statement::Expr(ast::Expression::Symbol(String::from("x")))),
        Box::new(ast::Statement::Expr(ast::Expression::Symbol(String::from("z")))),
    ]));
}
