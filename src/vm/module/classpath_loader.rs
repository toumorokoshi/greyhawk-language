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
use std::env;
use glob::glob;
use super::{load, Module};
use super::super::{VM, VMError, VMResult};
use yaml_rust::{Yaml, YamlLoader};
use std::fs::File;
use std::io::prelude::*;

static CLASSPATH_VAR: &'static str = "GH_PATH";

pub fn find_module_from_classpath(vm: &mut VM, name: &String) -> VMResult<Module> {
    let path = match env::var(CLASSPATH_VAR) {
        Ok(val) => val.clone(),
        Err(_) => String::from("./std")
    };
    for p in path.clone().split(":").collect::<Vec<&str>>() {
        if let Ok(module) = find_module_in_dir(vm, p, name) {
            return Ok(module);
        }
    }
    Err(VMError::new("no module found."))
}

pub fn find_module_in_dir(vm: &mut VM, dir: &str, name: &String) -> VMResult<Module> {
    let path = String::from(dir) + "/*.ghc";
    for entry in glob(&path).expect("failed to read glob pattern.") {
        match entry {
            Ok(entry_path) => {
                match entry_path.to_str() {
                    Some(valid_path) =>  {
                        let mut file = File::open(valid_path).unwrap();
                        let mut content = String::new();
                        file.read_to_string(&mut content).unwrap();
                        let module_name: String = valid_path.chars().skip(0).take(valid_path.len() - 4).collect();
                        if name == &module_name {
                            let ref yaml = YamlLoader::load_from_str(&content).unwrap()[0];
                            return load::load_module(vm, yaml);
                        }
                    },
                    None => { return Err(VMError::new("unable to parse path entry as string.")); }
                }
            },
            Err(e) => { return Err(VMError::new("unable to parse glob entry.")); }
       }
    }
    Err(VMError::new("no module found"))
}
