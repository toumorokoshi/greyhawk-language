use super::lexer;
use super::parser;
use super::codegen;
use super::vm;
use std;
use std::io::{self, Write};
use std::process;

pub fn start_repl() {
    println!("Greyhawk 0.0.3");
    let mut vm_instance = vm::VM::new();
    repl(&mut vm_instance);
}

fn repl(vm_instance: &mut vm::VM) {
    let lexer = lexer::Lexer::new();
    loop {
        std::io::stdout().write(b">>> ");
        std::io::stdout().flush();
        let mut input = String::new();
        io::stdin().read_line(&mut input).ok().expect("Failed to read line");
        let tokens = lexer.read(&input);
        if tokens.len() == 0 {
            process::exit(0);
        }
        println!("{}", tokens.len());
        match parser::parse(&tokens) {
            Ok(expressions) => {
                let function = codegen::generate_ops(&expressions);
                match &function {
                    &vm::function::Function::VMFunction(ref f) => {
                        println!("{}", f.scope);
                    },
                    _ => {},
                }
                let object = vm_instance.execute_function(&function, &[]);
                vm::print(&[object]);
            },
            Err(err) => {
                println!("{}", err);
            }
        }
    }
}