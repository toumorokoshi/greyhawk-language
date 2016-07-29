extern crate yaml_rust;
use super::super::evaluate_expr;
use vm::Op;
use vm::types;
use vm::scope;
use ast::{BinOp, BinaryOperator};

pub fn generate_binop(binop: &BinOp, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
    let left = evaluate_expr(&binop.left, scope, instructions);
    let right = evaluate_expr(&binop.right, scope, instructions);
    return if left.typ == types::FLOAT_TYPE.clone() {
        let object = scope.allocate_local(types::FLOAT_TYPE.clone());
        match binop.op {
            BinaryOperator::Plus => instructions.push(Op::FloatAdd{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Sub => instructions.push(Op::FloatSub{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Mul => instructions.push(Op::FloatMul{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Div => instructions.push(Op::FloatDiv{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Neq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                let second_cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::BoolNot{source: cmp.index, target: second_cmp.index});
                return second_cmp;
            },
            BinaryOperator::Eq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Le => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatLessThan{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Leq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatLessEqual{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Ge => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatLessThan{rhs: left.index, lhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Geq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::FloatLessEqual{rhs: left.index, lhs: right.index, target: cmp.index});
                return cmp;
            },
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
        };
        object
    } else {
        let object = scope.allocate_local(types::INT_TYPE.clone());
        match binop.op {
            BinaryOperator::Plus => instructions.push(Op::IntAdd{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Sub => instructions.push(Op::IntSub{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Mul => instructions.push(Op::IntMul{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Div => instructions.push(Op::IntDiv{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Neq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                let second_cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::BoolNot{source: cmp.index, target: second_cmp.index});
                return second_cmp;
            },
            BinaryOperator::Eq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Le => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntLessThan{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Leq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntLessEqual{lhs: left.index, rhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Ge => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntLessThan{rhs: left.index, lhs: right.index, target: cmp.index});
                return cmp;
            },
            BinaryOperator::Geq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                instructions.push(Op::IntLessEqual{rhs: left.index, lhs: right.index, target: cmp.index});
                return cmp;
            }
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
        };
        object
    };
}
