extern crate yaml_rust;
use super::{BinOp, Expression, Statement};
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn to_yaml(stmts: &Vec<Box<Statement>>) -> Yaml {
    let mut yaml = Vec::new();
    for stmt in stmts {
        yaml.push(stmt_to_yaml(&stmt));
    }
    return Yaml::Array(yaml);
}

pub fn stmt_to_yaml(stmt: &Statement) -> Yaml {
    let mut yaml = BTreeMap::new();
    match stmt {
        &Statement::Assignment(ref a) =>  {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("assignment".to_string()));
            yaml.insert(Yaml::String("name".to_string()), Yaml::String(a.name.clone()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(&a.expression));
            Yaml::Hash(yaml)
        },
        &Statement::FunctionDecl(ref func_decl) => {
            yaml.insert(Yaml::String("type".to_string()),
                        Yaml::String("function declaration".to_string()));
            yaml.insert(Yaml::String("name".to_string()),
                        Yaml::String(func_decl.name.clone()));
            yaml.insert(Yaml::String("statements".to_string()),
                        to_yaml(&func_decl.statements));
            Yaml::Hash(yaml)
        },
        &Statement::Return(ref expr) => {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("return".to_string()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(&expr));
            Yaml::Hash(yaml)
        },
        &Statement::Expr(ref expr) => expr_to_yaml(&expr),
        &Statement::Declaration(ref d) =>  {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("declaration".to_string()));
            yaml.insert(Yaml::String("name".to_string()), Yaml::String(d.name.clone()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(&d.expression));
            Yaml::Hash(yaml)
        },
        &Statement::While(ref w) =>  {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("while".to_string()));
            yaml.insert(Yaml::String("condition".to_string()), expr_to_yaml(&w.condition));
            yaml.insert(Yaml::String("block".to_string()), to_yaml(&w.block));
            Yaml::Hash(yaml)
        },
        &Statement::Import(ref i) => {
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("import".to_string()));
            yaml.insert(Yaml::String("module_name".to_string()), Yaml::String(i.module_name.clone()));
            yaml.insert(Yaml::String("block".to_string()), Yaml::String(i.name.clone()));
            Yaml::Hash(yaml)
        }
    }
}

pub fn expr_to_yaml(expr: &Expression) -> Yaml {
    match expr {
        &Expression::ArrayCreate(ref ac) => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("array".to_string()));
            yaml.insert(Yaml::String("length".to_string()), expr_to_yaml(&ac.size));
            let mut values = Vec::new();
            for ref v in &(ac.values) {
                values.push(expr_to_yaml(v));
            }
            yaml.insert(Yaml::String("values".to_string()), Yaml::Array(values));
            Yaml::Hash(yaml)
        },
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
        &Expression::Call{ref name, ref args} => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
            yaml.insert(Yaml::String("name".to_string()), Yaml::String(name.clone()));
            let mut arg_yaml = Vec::new();
            for arg in args {
                arg_yaml.push(expr_to_yaml(arg));
            }
            yaml.insert(Yaml::String("args".to_string()), Yaml::Array(arg_yaml));
            Yaml::Hash(yaml)
        },
        &Expression::Condition(ref c) => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("condition".to_string()));
            yaml.insert(Yaml::String("expression".to_string()), expr_to_yaml(&(c.expression)));
            yaml.insert(Yaml::String("true_block".to_string()), to_yaml(&c.true_block));
            yaml.insert(Yaml::String("false_block".to_string()), to_yaml(&c.false_block));
            Yaml::Hash(yaml)
        },
        &Expression::ItemGet(ref ig) => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("index_get".to_string()));
            yaml.insert(Yaml::String("source".to_string()), expr_to_yaml(&(ig.source)));
            yaml.insert(Yaml::String("index".to_string()), expr_to_yaml(&ig.index));
            Yaml::Hash(yaml)
        },
        &Expression::ItemSet(ref is) => {
            let mut yaml = BTreeMap::new();
            yaml.insert(Yaml::String("type".to_string()), Yaml::String("index_set".to_string()));
            yaml.insert(Yaml::String("target".to_string()), expr_to_yaml(&(is.target)));
            yaml.insert(Yaml::String("index".to_string()), expr_to_yaml(&is.index));
            yaml.insert(Yaml::String("value".to_string()), expr_to_yaml(&(is.value)));
            Yaml::Hash(yaml)
        },
        &Expression::Symbol(ref s) => Yaml::String(format!("symbol: {}", s)),
    }
}
