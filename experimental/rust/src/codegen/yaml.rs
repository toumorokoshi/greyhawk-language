extern crate yaml_rust;
use yaml_rust::{Yaml};

pub fn to_yaml(expressions: Vec<Box<super::Expression>>) -> Yaml {
    let mut yaml = Vec::new();
    for expr in expressions {
        yaml.push(expr_to_yaml(expr));
    }
    return Yaml::Array(yaml);
}

pub fn expr_to_yaml(expression: Box<super::Expression>) -> Yaml {
    match expression {
    }
    return Yaml::Integer(10);
}
