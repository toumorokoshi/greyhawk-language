extern crate yaml_rust;
mod function_declaration;
mod return_declaration;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;
use super::expressions::Expression;
pub use self::function_declaration::FunctionDeclaration;
pub use self::return_declaration::Return;

pub enum Statement {
    Func(FunctionDeclaration),
    Return(Return),
    Expr(Expression),
}

pub trait StatementBase {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>);
    fn to_yaml(&self) -> Yaml;
}
