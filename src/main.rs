#![feature(plugin, box_syntax, box_patterns)]
#![plugin(peg_syntax_ext)]
peg_file! peg_grammar("grammar.rustpeg");

#[macro_use]
extern crate lazy_static;
extern crate getopts;
extern crate yaml_rust;
use std::io::prelude::*;
use std::fs::File;
mod ast;

// mod lexer;
// mod parser;
mod codegen;
mod vm;
mod repl;
#[cfg(test)]
mod grammar_tests;
use yaml_rust::{Yaml};

use yaml_rust::emitter::YamlEmitter;
use std::env;

fn main () {
    let args: Vec<String> = env::args().collect();
    let matches = setup_opts().parse(&args[1..]).unwrap();
    match matches.free.len() {
        l if l > 1 => {
            match matches.free[0].as_ref() {
                "ast" => parse(&matches.free[1]),
                "ops" => print_ops(&matches.free[1]),
                "compile" => print_module(&matches.free[1]),
                _ => println!("no such subcommand"),
            }
        },
        1 => execute_file(&matches.free[0]),
        0 => {repl::start_repl();},
        _ => println!("Invalid Arg Count"),
    }
}

fn setup_opts() -> getopts::Options {
    let mut opts = getopts::Options::new();
    opts.optflag("h", "help", "print this help menu");
    return opts;
}

/*
Currently, lexing and parsing using the
native parser is removed in favor of using rust-peg.

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

fn print_ops(path: &String) {
    let mut content = String::new();
    let mut file = File::open(path).unwrap();
    file.read_to_string(&mut content).unwrap();
    let statement_list = peg_grammar::module(&content).unwrap();
    let module = codegen::gen_module_builder(&statement_list);
    module.print_ops();
}

fn print_module(path: &String) {
    let mut content = String::new();
    let mut file = File::open(path).unwrap();
    file.read_to_string(&mut content).unwrap();
    let statements = peg_grammar::module(&content).unwrap();
    let module_builder = codegen::gen_module_builder(&statements);
    let module = vm_instance.build_module(&("main".to_string()), &module_builder);
    let module_yaml = vm::dump_module(module);
    print_yaml(module_yaml);
}

fn execute_file(path: &String) {
    let mut vm_instance = vm::VM::new();
    let mut file = File::open(path).unwrap();
    let mut content = String::new();
    file.read_to_string(&mut content).unwrap();
    let statements = peg_grammar::module(&content).unwrap();
    let module = codegen::gen_module_builder(&statements);
    vm_instance.build_module(&("main".to_string()), &module);
}
