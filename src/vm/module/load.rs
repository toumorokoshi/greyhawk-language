use super::{Module};
use super::super::{scope, VM, types, VMError};
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn load_module(vm: &mut VM, root: &Yaml) -> VMResult<Module> {
}

pub fn load_module(vm: &mut VM, root: &Yaml) -> Result<Module, String> {
    match root {
        Hash(hash) => {
            if let Some(scope_instance_yaml) = hash.get("scope_instance".to_string()) {
                if let Some(scope_yaml) = hash.get("scope".to_string()) {
                    let scope = load_scope(vm, scope_yaml);
                    let scope_instance = load_scope_instance(vm, scope_instance_yaml);
                } else {
                    Err("unable to find scope".to_string())
                }
            } else {
                Err("unable to find scope_instance".to_string())
            }
        },
        _ => Err("expected a hash for module root.")
    }
}

fn load_scope(vm: &mut VM, scope_yaml: &Yaml) -> Result<Scope, String> {
    match scope_yaml {
        Hash(hash) => {
            if let Some(locals_yaml) => match hash.get() {
                if let Some(functions) => {
                }
            } else {
                Err("unable")
            }
        },
        _ => Err("expected a hash ")
    }
}

fn load_scope_instance(vm: &mut VM, s: &scope::Scope, si: &scope::ScopeInstance) -> Yaml {
    let mut root = BTreeMap::new();

    let mut registers = Vec::new();
    for i in 0..si.registers.len() {
        let ref typ = s.types[i];
        let obj = si.registers[i];
        let result =
            if *typ == *types::STRING_TYPE {
                let value = vm.get_string(obj as usize);
                Yaml::String((*value).clone())
            } else {
                Yaml::Integer(obj.clone())
            };
        registers.push(result);
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

fn load_scope(s: &scope::Scope) -> Yaml {
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
