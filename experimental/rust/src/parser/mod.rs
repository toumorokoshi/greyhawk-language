use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;

pub type ExprResult = Result<Box<codegen::Expression>, &'static str>;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<codegen::Expression>> {
    let mut token_iter = tokens.iter().peekable();
    let mut expressions: Vec<Box<codegen::expressions::Expression>> = Vec::new();
    let expr = parse_expression(&mut tokens.iter().peekable());
    match expr {
        Ok(e) => expressions.push(e),
        Err(err) => {},
    }
    return expressions;
}

pub fn parse_expression(tokens: &mut Peekable<Iter<lexer::Token>>) -> ExprResult {
    return match tokens.peek() {
        Some(&t) => {
            match t.typ {
                TokenType::Int(i) => return Ok(Box::new(codegen::IntExpression{value: i})),
                _ => Err("foo"),
            }
        },
        None => Err("foo"),
    }
}

/* pub fn parse_binary_operation<I: Iterator<Item=lexer::Token>> (tokens: &mut Peekable<I>) -> Box<codegen::Expression> {
    let mut left = parse_value(&mut tokens);
    // Box::new(codegen::IntExpression{value: 10}));
}

pub fn parse_value<I: Iterator<Item=lexer::Token>> (tokens: &mut Peekable<I>) -> Box<codegen::Expression> {
    return parse_base_value(&mut tokens);
} */

/* pub fn parse_base_value<I: Iterator<Item=lexer::Token>> (tokens: &mut Peekable<I>) -> Box<codegen::Expression> {
    match tokens.peek() {
        Some(t) =>,
        None =>
    }
    let next = tokens.peek();
    return match next.tok {
        // Int(i) => return Box::new(
    }
} */
