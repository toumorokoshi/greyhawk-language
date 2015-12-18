extern crate yaml_rust;
use super::super::evaluate_expr;
use lexer::token::TokenType;
use vm::Op;
use vm::types;
use vm::scope;
use std::collections::BTreeMap;
use yaml_rust::Yaml;
use ast::BinOp;

pub fn generate_binop(binop: &BinOp, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
    let left = evaluate_expr(&binop.left, scope, instructions);
    let right = evaluate_expr(&binop.right, scope, instructions);
    return if (left.typ == types::get_float_type()) {
        let object = scope.allocate_local(types::get_float_type());
        match binop.op {
            TokenType::Plus => instructions.push(Op::FloatAdd{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Sub => instructions.push(Op::FloatSub{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Mul => instructions.push(Op::FloatMul{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Div => instructions.push(Op::FloatDiv{lhs: left.index, rhs: right.index, target: object.index}),
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
            _ => {},
        };
        object
    } else {
        let object = scope.allocate_local(types::get_int_type());
        match binop.op {
            TokenType::Plus => instructions.push(Op::IntAdd{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Sub => instructions.push(Op::IntSub{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Mul => instructions.push(Op::IntMul{lhs: left.index, rhs: right.index, target: object.index}),
            TokenType::Div => instructions.push(Op::IntDiv{lhs: left.index, rhs: right.index, target: object.index}),
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
            _ => {},
        };
        object
    };
}
