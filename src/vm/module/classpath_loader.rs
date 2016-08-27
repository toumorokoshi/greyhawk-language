///
/// looks in the classpath for a file with the same name
/// as the specified file. If so, reads the file, and
/// returns the contents.
///
///
///
///
///
///
use lib::find_file_in_classpath;
use super::{load, Module};
use super::super::{VM, VMError, VMResult};
use yaml_rust::{YamlLoader};
use std::fs::File;
use std::io::prelude::*;

pub fn find_module_from_classpath(vm: &mut VM, name: &String) -> VMResult<Module> {
    match find_file_in_classpath(name, "ghc") {
        Some(path) => {
            let mut file = File::open(path).unwrap();
            let mut content = String::new();
            file.read_to_string(&mut content).unwrap();
            let ref yaml = YamlLoader::load_from_str(&content).unwrap()[0];
            return load::load_module(vm, yaml);
        },
        None => Err(VMError::new("no module found."))
    }
}
