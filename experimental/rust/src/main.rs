mod vm;
mod lexer;

use std::io::{self, Write};
use std::collections::HashMap;

fn main () {
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
    loop {
        std::io::stdout().write(b">>> ");
        std::io::stdout().flush();
        let mut guess = String::new();
        io::stdin().read_line(&mut guess).ok().expect("Failed to read line");
        let ops = &[
            vm::Op::AddInt { lhs: 0, rhs: 1 },
            vm::Op::ExecuteFunction { name: "foo"}
        ];
        let value = vm_instance.execute_instructions(module, ops);
        printValue(value);
    }
}

fn printValue(value: vm::Value) {
    match value {
        vm::Value::Int(intVal) => println!("{}", intVal),
        vm::Value::String(strVal) => println!("{}", strVal),
        vm::Value::None => println!("None")
    }
}
