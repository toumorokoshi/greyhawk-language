extern crate yaml_rust;
// pub mod statements;
// pub mod expressions;
use ast::Statement;
// pub mod yaml;
use super::vm;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;
use std::rc::Rc;

// pub use self::yaml::to_yaml;

pub fn generate_ops(statements: &Vec<Statement>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    for statement in statements {
        statement.evaluate(&mut scope, &mut ops);
    }
    return vm::Function::VMFunction(vm::VMFunction {
        scope: scope,
        ops: ops
    });
}

pub fn to_yaml(expressions: Vec<Statement>) -> Yaml {
    let mut yaml = Vec::new();
    return Yaml::Array(yaml);
}
