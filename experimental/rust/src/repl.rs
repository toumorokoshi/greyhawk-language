use std::mem;
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
        match &function {
            &vm::function::Function::VMFunction(ref f) => {
                println!("{}", f.scope);
            },
            _ => {},
        }
        let object = vm_instance.execute_function(module, &function);
        print_value(object);
    }
}

fn print_value(object: vm::Object) {
    if (object.typ == vm::types::get_int_type()) {
        println!("int: {}", object.value);
    } else if (object.typ == vm::types::get_float_type()) {
        unsafe {
            println!("float: {}", mem::transmute::<i32, f32>(object.value));
        }
    }
}
