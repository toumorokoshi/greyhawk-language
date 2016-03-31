extern crate yaml_rust;
use super::{BinOp, Expression, Statement};
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn to_yaml(stmts: Vec<Box<Statement>>) -> Yaml {
    let mut yaml = Vec::new();
    for stmt in stmts {
        yaml.push(stmt_to_yaml(&stmt));
    }
    return Yaml::Array(yaml);
}

pub fn stmt_to_yaml(stmt: &Statement) -> Yaml {
    let mut yaml = BTreeMap::new();
    match stmt {
        &Statement::FunctionDecl(ref func_decl) => {
            yaml.insert(Yaml::String("type".to_string()),
                        Yaml::String("function declaration".to_string()));
            Yaml::Hash(yaml)
        },
        &Statement::Return(ref expr) => {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("return".to_string()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(&expr));
            Yaml::Hash(yaml)
        },
        &Statement::Expr(ref expr) => expr_to_yaml(&expr),
    }
}

pub fn expr_to_yaml(expr: &Expression) -> Yaml {
    match expr {
        &Expression::ConstInt{value} => Yaml::Integer(value as i64),
        &Expression::ConstFloat{value} => Yaml::Real(value.to_string()),
        &Expression::ConstString{ref value} => Yaml::String(value.clone()),
        &Expression::BinOp(BinOp{ref op, ref left, ref right}) => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("binop".to_string()));
            yaml.insert(Yaml::String("op".to_string()), Yaml::String(format!("{}", op)));
            yaml.insert(Yaml::String("left".to_string()), expr_to_yaml(&left));
            yaml.insert(Yaml::String("right".to_string()), expr_to_yaml(&right));
            Yaml::Hash(yaml)
        },
        &Expression::Call{ref name, ref arg} => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
            yaml.insert(Yaml::String("name".to_string()), Yaml::String(name.clone()));
            yaml.insert(Yaml::String("arg".to_string()), expr_to_yaml(&arg));
            Yaml::Hash(yaml)
        }
    }
}
