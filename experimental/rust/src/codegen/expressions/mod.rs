extern crate yaml_rust;
use yaml_rust::Yaml;
use vm::scope;
use vm::Op;
mod intexpr;
mod floatexpr;
mod binop;
mod call;

pub use self::intexpr::IntExpression;
pub use self::floatexpr::FloatExpression;
pub use self::binop::BinOpExpression;
pub use self::call::CallExpression;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
    fn to_yaml(&self) -> Yaml;
}
