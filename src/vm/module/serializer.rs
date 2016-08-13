use super::{Module};
use super::super::scope;
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn dump_module(m: &Module) -> Yaml {
    let mut root = BTreeMap::new();
    root.insert(Yaml::String("scope_instance".to_string()), dump_scope_instance(&m.scope_instance));
    root.insert(Yaml::String("scope".to_string()), dump_scope(&m.scope));
    Yaml::Hash(root)
}

fn dump_scope_instance(si: &scope::ScopeInstance) -> Yaml {
    let mut root = BTreeMap::new();

    let mut registers = Vec::new();
    for r in &(si.registers) {
        registers.push(Yaml::Integer(r.clone() as i64));
    }
    root.insert(Yaml::String("registers".to_string()), Yaml::Array(registers));

    let mut arrays = Vec::new();
    for ar in &(si.arrays) {
        let mut dumped_array = Vec::new();
        for e in ar {
            dumped_array.push(Yaml::Integer(e.clone() as i64));
        }
        arrays.push(Yaml::Array(dumped_array));
    }
    root.insert(Yaml::String("arrays".to_string()), Yaml::Array(arrays));

    Yaml::Hash(root)
}

fn dump_scope(s: &scope::Scope) -> Yaml {
    let mut root = BTreeMap::new();

    let mut locals = BTreeMap::new();
    for (k, v) in &(s.locals) {
        locals.insert(Yaml::String(k.clone()), Yaml::Integer(v.clone() as i64));
    }
    root.insert(Yaml::String("locals".to_string()), Yaml::Hash(locals));

    let mut types = Vec::new();
    for ref t in &(s.types) {
        types.push(Yaml::String(t.name.clone()));
    }
    root.insert(Yaml::String("types".to_string()), Yaml::Array(types));

    Yaml::Hash(root)
}
