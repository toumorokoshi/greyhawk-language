macro_rules! unpack_yaml {
    ($expr:expr, $expected:path , $err:expr) => (match $expr {
        $expected(cast_expr) => {cast_expr},
        _ => {return Err(VMError::new($err))}
    })
}

macro_rules! extract_from_key {
    ($yaml_hash:expr, $string:expr) => (match $yaml_hash.get(&Yaml::String(String::from($string))) {
        Some(e) => e.clone(),
        // None => {return Err(VMError::new(format!("expected key {}", $string)))}
        None => {return Err(VMError::new(""))}
    })
}

macro_rules! extract_and_unpack {
    ($yaml_hash:expr, $key:expr, $expected:path, $err:expr) => (
        unpack_yaml!(extract_from_key!($yaml_hash, $key), $expected, $err)
    )
}

use super::{Module};
use super::super::{scope, VM, types, VMError, VMResult};
use std::collections::BTreeMap;
use std::rc::Rc;
use yaml_rust::{Yaml};

pub fn load_module(vm: &mut VM, root: &Yaml) -> VMResult<Module> {
    let hash = unpack_yaml!(root.clone(), Yaml::Hash, "yaml object is not a hash.");

    let scope_yaml = extract_and_unpack!(hash, "scope", Yaml::Hash, "yaml object is not a hash.");
    let scope = try!(load_scope(vm, &scope_yaml));

    let scope_instance_yaml = extract_and_unpack!(hash, "scope_instance", Yaml::Hash, "yaml object is not a hash.");
    let scope_instance = try!(load_scope_instance(vm, &scope, &scope_instance_yaml));

    Ok(Module{scope: Rc::new(scope), scope_instance: scope_instance})
}

fn load_scope(_: &mut VM, scope_yaml: &BTreeMap<Yaml, Yaml>) -> VMResult<scope::Scope> {
    let mut scope = scope::Scope::new(None);
    let locals_yaml = extract_and_unpack!(scope_yaml.clone(), "locals", Yaml::Hash, "yaml object is not a hash.");
    for (name, value) in locals_yaml {
        match (name, value) {
            (Yaml::String(s), Yaml::Integer(i)) => {
                scope.locals.insert(s, i as usize);
            },
            _ => { return Err(VMError::new("unable to parse locals.")); }
        }
    }
    let types_yaml = extract_and_unpack!(scope_yaml.clone(), "types", Yaml::Array, "yaml object is not an array.");
    for t_string in types_yaml {
        match t_string {
            Yaml::String(s) => {
                let t = types::get_type_ref_from_string(&s);
                scope.types.push(t);
            },
            _ => { return Err(VMError::new("unable to parse types")) }
        }
    }
    Ok(scope)
}

fn load_scope_instance(vm: &mut VM, s: &scope::Scope, si_yaml: &BTreeMap<Yaml, Yaml>) -> VMResult<scope::ScopeInstance> {
    let mut scope_instance = s.create_instance();
    let registers = extract_and_unpack!(si_yaml.clone(), "registers", Yaml::Array, "registers is not an array");
    for i in 0..registers.len() {
        match registers[i] {
            Yaml::Integer(value) => {
                scope_instance.registers[i] = value;
            },
            Yaml::String(ref s) => {
                let value = vm.add_string(&s);
                scope_instance.registers.push(value as i64);
            },
            _ => { return Err(VMError::new("unable to parse registers in scope instance")); }
        }
    }
    let arrays = extract_and_unpack!(si_yaml.clone(), "arrays", Yaml::Array, "arrays is not an array");
    for r in arrays {
        match r {
            Yaml::Array(r_arr) => {
                let mut new_arr = vec![];
                for r_arr_elem in r_arr {
                    match r_arr_elem {
                        Yaml::Integer(i) => {
                            new_arr.push(i as usize);
                        }
                        _ => { return Err(VMError::new("unable to parse arrays.")); }
                    }
                }
            },
            _ => { return Err(VMError::new("unable to parse arrays in scope instance")); }
        }
    }
    Ok(scope_instance)
}
