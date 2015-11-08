extern crate yaml_rust;
mod function_declaration;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;
pub use self::function_declaration::FunctionDeclaration;

pub trait StatementBase {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>);
    fn to_yaml(&self) -> Yaml;
}
