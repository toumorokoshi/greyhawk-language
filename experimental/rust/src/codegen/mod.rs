pub mod statements;
pub mod expressions;
pub mod scope;
use super::vm;
use std::rc::Rc;

pub use self::expressions::Expression;
pub use self::expressions::IntExpression;

pub fn generate_ops(expressions: &Vec<Box<Expression>>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = scope::Scope::new();
    for expression in expressions {
        expression.generate(&mut scope, &mut ops);
    }
    ops.push(vm::ops::Op::Return{register: 0});
    return vm::Function::VMFunction(vm::VMFunction {
        register_count: scope.local_count,
        ops: ops
    });
}
