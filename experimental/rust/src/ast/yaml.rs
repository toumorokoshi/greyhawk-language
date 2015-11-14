extern crate yaml_rust;
use super::{Expression, Statement};
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn to_yaml(stmts: Vec<Statement>) -> Yaml {
    let mut yaml = Vec::new();
    for stmt in stmts {
        yaml.push(stmt_to_yaml(stmt));
    }
    return Yaml::Array(yaml);
}

pub fn stmt_to_yaml(stmt: Statement) -> Yaml {
    let mut yaml = BTreeMap::new();
    match stmt {
        Statement::FunctionDecl{name, statements} => {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
            Yaml::Hash(yaml)
        },
        Statement::Return(expr) => {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("return".to_string()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(expr));
            Yaml::Hash(yaml)
        },
        Expr(expr) => expr_to_yaml(expr),
    }
}

pub fn expr_to_yaml(expr: Expression) -> Yaml {
    match expr {
        Expression::ConstInt{value} => Yaml::Integer(value as i64),
        Expression::ConstFloat{value} => Yaml::Real(value.to_string()),
        Expression::BinOp{op, left, right} => {
        },
        Expression::BinOp{name, arg} => {
        }
    }
}
