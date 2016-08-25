use std::env;
use glob::glob;
use std::path::PathBuf;

static CLASSPATH_VAR: &'static str = "GH_PATH";

pub fn find_file_in_classpath(name: &String, suffix: &str) -> Option<String> {
    let path = env::var(CLASSPATH_VAR).unwrap_or(String::from("./std"));
    for p in path.clone().split(":").collect::<Vec<&str>>() {
        if let Some(file_path) = find_file_in_dir(p, name, suffix) {
            return Some(file_path);
        }
    }
    None
}

fn find_file_in_dir(dir: &str, name: &String, suffix: &str) -> Option<String> {
    let mut path = PathBuf::from(dir);
    path.push(name);
    path.set_extension(suffix);
    match path.exists() {
        true => match path.to_str() {
            Some(s) => Some(String::from(s)),
            None => None
        },
        false => None
    }
}
