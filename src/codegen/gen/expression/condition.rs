use ast::{Condition};
use vm::{Scope, LocalObject, Op, VM};
use super::super::{evaluate_expr, gen_statement_list};
use codegen::{Context, CGError, CGResult};
use super::super::{gen_expression};

pub fn gen_condition(c: &mut Context, c: &Condition) -> CGResult<LocalObject> {
    let cond_object = try!(gen_expression(c, &c.expression));
    let cond_index = ops.len();
    // push a noop, replace it later with the correct branch.
    ops.push(Op::Noop{});
    try!(gen_statement_list(c, &(c.true_block), scope, ops));
    let end_true_block_index = ops.len();
    ops.push(Op::Noop{});
    try!(gen_statement_list(c, &(c.false_block)));
    let end_false_block_index = ops.len();
    ops[cond_index] = Op::Branch{
        condition: cond_object.index, if_false: end_true_block_index + 1
    };
    ops[end_true_block_index] = Op::Goto{
        position: end_false_block_index
    };
    Ok(cond_object)
}
