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
use super::load;
use super::super::{VM, VMError, VMResult};
use yaml_rust::{Yaml, YamlLoader};
use std::fs::File;

static ref CLASSPATH_VAR: &'str = "GH_PATH";

pub fn find_module_from_classpath(vm: &mut VM, name: &String) -> VMResult<Module> {
    let path = match env::var(CLASSPATH_VAR) {
        Ok(val) => val.split(":").collect::<Vec<String>>(),
        Err(_) => vec![String::from("./std")]
    }
    for p in path {
        if let Some(module) = find_module_in_dir(p, name) {
            return module;
        }
    }
    None
}

pub fn find_module_in_dir(vm: &mut VM, dir: &String, name: &String) -> VMResult<Module> {
    let path = dir + "/*.ghc"
    for entry in glob(path) {
        let mut file = File::open(path).unwrap();
        let mut content = String::new();
        file.read_to_string(&mut content).unwrap();
        let module_name = entry[..(entry.len() - 4)];
        if module_name == name {
            let yaml = YamlLoader::load_from_str(s).unwrap();
            return try!(load::load_module(vm, yaml));
        }
    }
    None
}
