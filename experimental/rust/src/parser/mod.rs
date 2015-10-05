use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::iter::Peekable;

type TokenIter = Peekable<Iterator<Item=lexer::Token>>;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<codegen::Expression>> {
    let mut token_iter = tokens.iter().peekable();
    let mut expressions: Vec<Box<codegen::expressions::Expression>> = Vec::new();
    expressions.push(parse_expression(token_iter));
    return expressions;
}

pub fn parse_expression(tokens: &mut TokenIter) -> Box<codegen::Expression> {
    return parse_binary_operation(tokens);
}

pub fn parse_binary_operation(tokens: &mut TokenIter) -> Box<codegen::Expression> {
    let mut left = parse_value();
    // Box::new(codegen::IntExpression{value: 10}));
}

pub fn parse_value(tokens: &mut TokenIter) -> Box<codegen::Expression> {
    return parse_base_value(tokens);
}

pub fn parse_base_value(tokens: &mut TokenIter) -> Box<codegen::Expression> {
    let next = tokens.peek();
    return match next.tok {
        // Int(i) => return Box::new(
    }
}
