use ast::{Condition};
use vm::{Scope, Op};
use super::evaluate_expr;

pub fn gen_condition(c: &Condition, scope: &mut Scope, ops: &mut Vec<Op>) -> scope::LocalObject {
    let cond_object = evaluate_expr(&(c.expression));
    // ops.push(Op::Branch{
    //     condition: cond_object.index,
    // });
}
