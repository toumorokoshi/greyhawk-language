use super::{ModuleBuilder, Module}
use super::super::scope;
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn dump_module(m: &Module) -> Yaml {
    let mut root = BTreeMap::new();
    root.insert("scope_instance", dump_scope_instance(&m.scope_instance));
    root.insert("scope", dump_scope(&m.scope));
    Yaml::Hash(root)
}

fn dump_scope_instance(si: &scope::ScopeInstance) -> Yaml {
    let mut root = BTreeMap::new();

    let mut registers = Vec::new();
    for r in si.registers {
        registers.push(Yaml::Integer(r as i64));
    }
    root.insert(Yaml::String("registers"), Yaml::Array(registers));

    let mut arrays = Vec::new();
    for ar in si.arrays {
        let dumped_array = Vec::new();
        for e in ar {
            dumped_array.push(Yaml::Integer(e));
        }
        arrays.push(Yaml::Array(dumped_array));
    }
    root.insert(Yaml::String("arrays": Yaml::Array(arrays)));

    Yaml::Hash(root)
}

fn dump_scope(s: &scope::Scope) -> Yaml {
    let mut root = BTreeMap::new();

    let mut locals = BTreeMap::new();
    for &k, &v in s.locals {
        locals.insert(k.clone(), v.clone());
    }
    root.insert(Yaml::String("locals"), Yaml::Hash(locals));

    let mut types = Vec::new();
    for t in s.types {
        types.push(t.name());
    }
    root.insert(Yaml::String("types"), Yaml::Array(types));

    Yaml::Hash(root)
}
