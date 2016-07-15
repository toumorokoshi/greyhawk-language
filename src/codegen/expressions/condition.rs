use ast::{Condition};
use vm::{Scope, LocalObject, Op};
use super::super::{evaluate_expr, gen_statement_list};

pub fn gen_condition(c: &Condition, scope: &mut Scope, ops: &mut Vec<Op>) -> LocalObject {
    let cond_object = evaluate_expr(&(c.expression), scope, ops);
    let cond_index = ops.len();
    // push a noop, replace it later with the correct branch.
    ops.push(Op::Noop{});
    gen_statement_list(&(c.true_block), scope, ops);
    let end_true_block_index = ops.len();
    ops.push(Op::Noop{});
    gen_statement_list(&(c.false_block), scope, ops);
    let end_false_block_index = ops.len();
    ops[cond_index] = Op::Branch{
        condition: cond_object.index, if_false: end_true_block_index + 1
    };
    ops[end_true_block_index] = Op::Goto{
        position: end_false_block_index
    };
    cond_object
}
