use super::ExprResult;
use std::slice::Iter;
use std::iter::Peekable;
use lexer;
use lexer::token::TokenType;
use codegen;

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
    let nextToken = tokens.next();
    return match nextToken {
        Some(t) => {
            match t.typ {
                TokenType::Int(i) => Ok(Box::new(codegen::IntExpression{value: i})),
                TokenType::Float(f) => Ok(Box::new(codegen::FloatExpression{value: f})),
                TokenType::Symbol(s) => Err("not implemented"),
                _ => Err("unable to find basic type."),
            }
        },
        None => Err("foo"),
    }
}


pub fn parse_call(name: String, tokens: &mut Peekable<Iter<lexer::Token>>) -> ExprResult {
    Err("not implemented.")
}
