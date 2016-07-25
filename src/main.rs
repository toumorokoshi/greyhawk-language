#![feature(plugin, box_syntax, box_patterns)]
#![plugin(peg_syntax_ext)]
peg_file! peg_grammar("grammar.rustpeg");

extern crate getopts;
extern crate yaml_rust;
use std::io::prelude::*;
use std::fs::File;
mod ast;

// mod lexer;
// mod parser;
mod codegen;
mod interpreter;
mod vm;
mod repl;
#[cfg(test)]
mod grammar_tests;
use yaml_rust::{Yaml};

use yaml_rust::emitter::YamlEmitter;
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
                "ast" => parse(&matches.free[1]),
                _ => println!("no such subcommand"),
            }
        },
        1 => execute_file(&matches.free[0]),
        0 => {repl::start_repl();},
        _ => println!("Invalid Args"),
    }
}

fn setup_opts() -> getopts::Options {
    let mut opts = getopts::Options::new();
    opts.optflag("h", "help", "print this help menu");
    return opts;
}

/*
Currently, lexing and parsing using the
native parser is deprecated in favor of using rust-peg.

fn lexer(path: &String) {
    println!("{}", path);
    let lexer = lexer::Lexer::new();

    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();

    let tokens = lexer.read(&content);
    for token in &tokens {
        println!("{}: {}", token.line_num, token.typ);
    }
}
*/

fn parse(path: &String) {
    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    match peg_grammar::module(&content) {
        Ok(statement_list) => {
            let yaml = ast::yaml::to_yaml(&statement_list);
            print_yaml(yaml);
        },
        Err(err) => {println!("{}", err)}
    }
}

fn print_yaml(yaml: Yaml) {
    let mut out_str = String::new();
    {
        let mut emitter = YamlEmitter::new(&mut out_str);
        emitter.dump(&yaml).unwrap();
    }
    println!("{}", out_str);
}

fn execute_file(path: &String) {
    let mut vm_instance = vm::VM::new();
    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    match peg_grammar::module(&content) {
        Ok(statement_list) => {
            let function = codegen::generate_ops(&statement_list);
            /* match &function {
                &vm::function::Function::VMFunction(ref f) => {
                    println!("{}", f.scope);
                },
                _ => {},
            } */
            vm_instance.execute_function(&function, &[]);
        },
        Err(msg) => {
            println!("{}", msg);
        }
    }
}
