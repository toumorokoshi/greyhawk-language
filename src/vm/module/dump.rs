use super::{Module};
use super::super::{scope, VM, types, Function};
use std::collections::BTreeMap;
use yaml_rust::{Yaml};

pub fn dump_module(vm: &VM, m: &Module) -> Yaml {
    let mut root = BTreeMap::new();
    root.insert(Yaml::String("scope_instance".to_string()), dump_scope_instance(&vm, &m.scope, &m.scope_instance));
    root.insert(Yaml::String("scope".to_string()), dump_scope(&m.scope)); Yaml::Hash(root)
}

fn dump_scope_instance(vm: &VM, s: &scope::Scope, si: &scope::ScopeInstance) -> Yaml {
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

    let mut functions = BTreeMap::new();
    for (k, v) in &(s.functions) {
        functions.insert(Yaml::String(k.clone()), dump_function(v));
    }

    Yaml::Hash(root)
}


fn dump_function(f: &Function) -> Yaml {
    match f {
        &Function::NativeFunction{name: _, function: _, typ: _} => {
        },
        &Function::VMFunction(_) => {
        }
    }
    Yaml::Hash(BTreeMap::new())
}
