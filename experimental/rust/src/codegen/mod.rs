pub mod statements;
pub mod expressions;
pub mod yaml;
use super::vm;
use std::rc::Rc;

pub use self::statements::Statement;
pub use self::expressions::Expression;
pub use self::expressions::CallExpression;
pub use self::expressions::FloatExpression;
pub use self::expressions::IntExpression;
pub use self::expressions::BinOpExpression;
pub use self::yaml::to_yaml;

pub fn generate_ops(expressions: &Vec<Box<Expression>>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    let mut result = None;
    for expression in expressions {
        result = Some(expression.generate(&mut scope, &mut ops));
    }
    match result {
        Some(object) => ops.push(vm::ops::Op::Return{register: object.index}),
        None => {},
    }
    return vm::Function::VMFunction(vm::VMFunction {
        scope: scope,
        ops: ops
    });
}
