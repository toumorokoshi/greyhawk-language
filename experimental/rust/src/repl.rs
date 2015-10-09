use super::lexer;
use super::parser;
use super::codegen;
use super::vm;
use std;
use std::io::{self, Write};

pub fn start_repl() {
    println!("Greyhawk 0.0.3");
    let vm_instance = vm::VM::new();
    match vm_instance.modules.get("main") {
        Some(module) => {
            repl(module, &vm_instance);
        },
        None => println!("unable to load main module!"),
    }
}

fn repl(module: &vm::Module, vm_instance: &vm::VM) {
    let lexer = lexer::Lexer::new();
    loop {
        std::io::stdout().write(b">>> ");
        std::io::stdout().flush();
        let mut input = String::new();
        io::stdin().read_line(&mut input).ok().expect("Failed to read line");
        let tokens = lexer.read(&input);
        let expressions = parser::parse(&tokens);
        let function = codegen::generate_ops(&expressions);
        let value = vm_instance.execute_function(module, &function);
        println!("{}", value);
    }
}
