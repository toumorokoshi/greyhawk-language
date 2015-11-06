extern crate yaml_rust;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;

pub trait Statement {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>);
    fn to_yaml(&self) -> Yaml;
}

pub struct FunctionDeclaration {
    name: str,
}
