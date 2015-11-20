extern crate yaml_rust;
use yaml_rust::Yaml;
use vm::scope;
use vm::Op;
mod binop;

pub use binop::generate_binop;
