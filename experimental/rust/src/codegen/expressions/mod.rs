use vm::scope;
use vm::Op;
use super::statements::Statement;
mod intexpr;
mod floatexpr;
mod binop;
mod call;

pub use self::intexpr::IntExpression;
pub use self::floatexpr::FloatExpression;
pub use self::binop::BinOpExpression;
pub use self::call::CallExpression;

pub trait Expression : Statement {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
}
