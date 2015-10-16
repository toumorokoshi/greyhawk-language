pub mod statements;
pub mod expressions;
pub mod scope;
use super::vm;
use std::rc::Rc;

pub use self::expressions::Expression;
pub use self::expressions::FloatExpression;
pub use self::expressions::IntExpression;
pub use self::expressions::AddExpression;

pub fn generate_ops(expressions: &Vec<Box<Expression>>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = scope::Scope::new();
    let mut result = None;
    for expression in expressions {
        result = Some(expression.generate(&mut scope, &mut ops));
    }
    match result {
        Some(object) => ops.push(vm::ops::Op::Return{register: object.index}),
        None => {},
    }
    return vm::Function::VMFunction(vm::VMFunction {
        register_count: scope.local_count,
        ops: ops
    });
}
