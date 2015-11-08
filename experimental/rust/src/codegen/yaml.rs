extern crate yaml_rust;
use yaml_rust::{Yaml};
use super::Statement;

pub fn to_yaml(expressions: Vec<Statement>) -> Yaml {
    let mut yaml = Vec::new();
    for expr in expressions {
        yaml.push(expr.to_yaml());
    }
    return Yaml::Array(yaml);
}
