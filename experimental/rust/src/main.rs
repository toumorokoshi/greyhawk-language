mod vm;

use std::io;
use std::collections::HashMap;

fn main () {
    println!("Greyhawk 0.0.3");
    let vm_instance = vm::VM::new();

    match vm_instance.modules.get("main") {
        Some(module) => {
            let ops = &[
                vm::Op::AddInt { lhs: 0, rhs: 1 },
                vm::Op::ExecuteFunction { name: "foo"}
            ];
            vm_instance.execute_instructions(module, ops);
        },
        None => println!("unable to load main module!"),
    }
}
