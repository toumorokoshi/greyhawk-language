use super::peg_grammar::{assignment, assign, expression, symbol};
use super::ast;
use lexer::token::{Token, TokenType};

#[test]
fn test_symbol() {
    assert_eq!(symbol("foo"), Ok(String::from("foo")));
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
fn test_assign() {
    assert_eq!(assign(":="), Ok(Token{typ: TokenType::Assign,  line_num: 1}));
    assert!(assign("!=").is_err());
}

#[test]
fn test_assignment() {
    assert_eq!(assignment("x := y"), Ok(
        ast::Assignment{target: String::from("x"),
                        expression: Box::new(ast::Expression::Symbol(String::from("y")))}
    ));
}
