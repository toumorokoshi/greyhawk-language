extern crate getopts;
extern crate yaml_rust;
use std::io::prelude::*;
use std::fs::File;
mod lexer;
mod parser;
mod codegen;
mod vm;
mod repl;

use yaml_rust::emitter::YamlEmitter;
use std::collections::HashMap;
use std::env;

fn main () {
    let args: Vec<String> = env::args().collect();
    let matches = match setup_opts().parse(&args[1..]) {
        Ok(m) => {m},
        Err(f) => {panic!(f.to_string())},
    };
    match matches.free.len() {
        l if l > 1 => {
            match matches.free[0].as_ref() {
                "lexer" => lexer(&matches.free[1]),
                "parser" => parse(&matches.free[1]),
                _ => panic!("no such command"),
            }
        },
        1 => execute_file(),
        0 => repl::start_repl(),
        _ => panic!("Invalid Args"),
    }
}

fn setup_opts() -> getopts::Options {
    let mut opts = getopts::Options::new();
    return opts;
}

fn lexer(path: &String) {
    println!("{}", path);
    let lexer = lexer::Lexer::new();

    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();

    let tokens = lexer.read(&content);
    for token in &tokens {
        println!("{}", token.typ);
    }
}

fn parse(path: &String) {
    let lexer = lexer::Lexer::new();

    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    let tokens = lexer.read(&content);
    let expressions = parser::parse(&tokens);
    let yaml = codegen::to_yaml(expressions);
    let mut out_str = String::new();
    {
        let mut emitter = YamlEmitter::new(&mut out_str);
        emitter.dump(&yaml).unwrap();
    }
    println!("{}", out_str);
}

fn execute_file() {
}
