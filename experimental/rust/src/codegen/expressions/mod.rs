extern crate yaml_rust;
use yaml_rust::Yaml;
use vm::scope;
use vm::Op;
pub mod binop;

pub use self::binop::generate_binop;
