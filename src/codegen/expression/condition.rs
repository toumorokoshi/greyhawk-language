use ast::{Condition};
use vm::{LocalObject, Op};
use super::super::{gen_statement_list};
use codegen::{Context, CGResult};
use super::{gen_expression};

pub fn gen_condition(c: &mut Context, cond: &Condition) -> CGResult<LocalObject> {
    let cond_object = try!(gen_expression(c, &cond.expression));
    let cond_index = c.block.ops.len();
    // push a noop, replace it later with the correct branch.
    c.block.ops.push(Op::Noop{});
    try!(gen_statement_list(c, &(cond.true_block)));
    let end_true_block_index = c.block.ops.len();
    c.block.ops.push(Op::Noop{});
    try!(gen_statement_list(c, &(cond.false_block)));
    let end_false_block_index = c.block.ops.len();
    c.block.ops[cond_index] = Op::Branch{
        condition: cond_object.index, if_false: end_true_block_index + 1
    };
    c.block.ops[end_true_block_index] = Op::Goto{
        position: end_false_block_index
    };
    Ok(cond_object)
}
