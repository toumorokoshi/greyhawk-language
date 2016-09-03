extern crate yaml_rust;
use super::super::evaluate_expr;
use vm::{types, scope, Op, VM};
use ast::{BinOp, BinaryOperator};
use codegen::{Context, CGError, CGResult};

pub fn generate_binop(c: &mut Codegen, binop: &BinOp) -> CGResult<LocalObject> {
    let left = try!(gen_expression(c, &binop.left));
    let right = try!(gen_expression(c, &binop.right));
    return if left.typ == types::FLOAT_TYPE.clone() {
        let object = scope.allocate_local(types::FLOAT_TYPE.clone());
        match binop.op {
            BinaryOperator::Plus => c.block.ops.push(Op::FloatAdd{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Sub  => c.block.ops.push(Op::FloatSub{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Mul  => c.block.ops.push(Op::FloatMul{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Div  => c.block.ops.push(Op::FloatDiv{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Neq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                let second_cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::BoolNot{source: cmp.index, target: second_cmp.index});
                return Ok(second_cmp);
            },
            BinaryOperator::Eq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Le => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatLessThan{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Leq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatLessEqual{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Ge => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatLessThan{rhs: left.index, lhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Geq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::FloatLessEqual{rhs: left.index, lhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
        };
        Ok(object)
    } else {
        let object = scope.allocate_local(types::INT_TYPE.clone());
        match binop.op {
            BinaryOperator::Plus => c.block.ops.push(Op::IntAdd{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Sub => c.block.ops.push(Op::IntSub{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Mul => c.block.ops.push(Op::IntMul{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Div => c.block.ops.push(Op::IntDiv{lhs: left.index, rhs: right.index, target: object.index}),
            BinaryOperator::Neq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                let second_cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::BoolNot{source: cmp.index, target: second_cmp.index});
                return Ok(second_cmp);
            },
            BinaryOperator::Eq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntCmp{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Le => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntLessThan{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Leq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntLessEqual{lhs: left.index, rhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Ge => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntLessThan{rhs: left.index, lhs: right.index, target: cmp.index});
                return Ok(cmp);
            },
            BinaryOperator::Geq => {
                let cmp = scope.allocate_local(types::BOOL_TYPE.clone());
                c.block.ops.push(Op::IntLessEqual{rhs: left.index, lhs: right.index, target: cmp.index});
                return Ok(cmp);
            }
            // TODO: this should be validated when creating the
            // expression, rather that during evaluation.
        };
        Ok(object)
    };
}
