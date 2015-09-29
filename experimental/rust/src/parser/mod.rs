use super::lexer;
use super::codegen;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<Box<codegen::Expression>> {
    let mut expressions: Vec<Box<codegen::expressions::Expression>> = Vec::new();
    expressions.push(Box::new(codegen::IntExpression{value: 10}));
    return expressions;
}
