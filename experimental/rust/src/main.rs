extern crate getopts;
mod lexer;
mod parser;
mod codegen;
mod vm;
mod repl;

use std::collections::HashMap;
use std::env;

fn main () {
    let args: Vec<String> = env::args().collect();
    let matches = match setup_opts().parse(&args[1..]) {
        Ok(m) => {m},
        Err(f) => {panic!(f.to_string())},
    };
    if matches.free.len() > 1 {
        match matches.free[0].as_ref() {
            "lexer" => lexer(&matches.free[1]),
            _ => panic!("no such command"),
        }
    } else {
        repl::start_repl();
    }
}

fn setup_opts() -> getopts::Options {
    let mut opts = getopts::Options::new();
    return opts;
}

fn lexer(path: &String) {
    println!("start lexer");
}
