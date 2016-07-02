#![feature(plugin)]
#![plugin(peg_syntax_ext)]
peg_file! peg_grammar("grammar.rustpeg");

extern crate getopts;
extern crate yaml_rust;
use std::io::prelude::*;
use std::fs::File;
mod ast;
mod lexer;
mod parser;
mod codegen;
mod vm;
mod repl;

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
                "lexer" => lexer(&matches.free[1]),
                "parser" => parse(&matches.free[1]),
                "peg" => peg(&matches.free[1]),
                _ => panic!("no such command"),
            }
        },
        1 => execute_file(&matches.free[0]),
        0 => repl::start_repl(),
        _ => panic!("Invalid Args"),
    }
}

fn setup_opts() -> getopts::Options {
    let mut opts = getopts::Options::new();
    opts.optflag("h", "help", "print this help menu");
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
        println!("{}: {}", token.line_num, token.typ);
    }
}

fn peg(path: &String) {
    match peg_grammar::symbol("foo") {
        Ok(v) => {println!("{}", v)},
        _ => {println!("An error ocurred!")}
    }
}

fn parse(path: &String) {
    let lexer = lexer::Lexer::new();

    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    let tokens = lexer.read(&content);
    match parser::parse(&tokens) {
        Ok(expressions) => {
            let yaml = ast::yaml::to_yaml(expressions);
            let mut out_str = String::new();
            {
                let mut emitter = YamlEmitter::new(&mut out_str);
                emitter.dump(&yaml).unwrap();
            }
            println!("{}", out_str);
        },
        Err(msg) => {
            println!("{}", msg);
        }
    }
}

fn execute_file(path: &String) {
    let lexer = lexer::Lexer::new();
    let mut vm_instance = vm::VM::new();

    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    let tokens = lexer.read(&content);
    match parser::parse(&tokens) {
        Ok(expressions) => {
            let function = codegen::generate_ops(&expressions);
            match &function {
                &vm::function::Function::VMFunction(ref f) => {
                    // println!("{}", f.scope);
                },
                _ => {},
            }
            vm_instance.execute_function(&function, &[]);
        },
        Err(msg) => {
            println!("{}", msg);
        }
    }
}
