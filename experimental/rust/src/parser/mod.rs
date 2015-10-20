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
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return expressions;
}

pub fn parse_expression(tokens: &mut Peekable<Iter<lexer::Token>>) -> ExprResult {
    return parse_binary_operation(tokens);
}

pub fn parse_binary_operation(tokens: &mut Peekable<Iter<lexer::Token>>) -> ExprResult {
    let left = parse_base_value(tokens);
    let token_type = match tokens.next() {
        Some(t) => {
            match t.typ {
                TokenType::Plus => TokenType::Plus,
                TokenType::Sub => TokenType::Sub,
                TokenType::Mul => TokenType::Mul,
                TokenType::Div => TokenType::Div,
                _ => return Err("unable to parse binary operation."),
            }
        },
        None => return Err("unable to parse binary operation."),
    };
    let right = parse_base_value(tokens);
    return match left {
        Ok(l) => match right {
            Ok(r) => Ok(Box::new(codegen::BinOpExpression{op: token_type, left: l, right: r})),
            Err(e) => Err(e),
        },
        Err(e) => Err(e),
    };
}

pub fn parse_base_value(tokens: &mut Peekable<Iter<lexer::Token>>) -> ExprResult {
    return match tokens.next() {
        Some(t) => {
            match t.typ {
                TokenType::Int(i) => Ok(Box::new(codegen::IntExpression{value: i})),
                TokenType::Float(f) => Ok(Box::new(codegen::FloatExpression{value: f})),
                _ => Err("unable to find basic type."),
            }
        },
        None => Err("foo"),
    }
}
