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

static ref CLASSPATH_VAR: &'str = "GH_PATH";

pub fn find_module_from_classpath(name: &String) -> Option<Module> {
    let path = match env::var(CLASSPATH_VAR) {
        Ok(val) => val.split(":").collect::<Vec<String>>(),
        Err(_) => vec![String::from("./std")]
    }
    for p in path {
        if let Some(module) = find_module_in_dir(p, name) {
            return module;
        }
    }
    return None
}

pub fn find_module_in_dir(dir: &String, name: &String) {
    let path = dir + "/*.ghc"
    for entry in glob(path) {
        let module_name = entry[..(entry.len() - 4)];
        if module_name == name {
            load::load_module();
        }
    }
}
