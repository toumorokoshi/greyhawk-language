mod vm;

use std::io;
use std::collections::HashMap;

fn main () {
    println!("Greyhawk 0.0.3");
    let vm_instance = vm::VM{ memory: 0};
    let ops = &[vm::Op::AddInt { lhs: 0, rhs: 1 }];
    vm_instance.execute_instructions(ops);
    /* loop {
        println!(">> ");
        let mut guess = String::new();
        io::stdin().read_line(&mut guess)
            .ok()
            .expect("Failed to read line");
        println!("received input: {}", guess);
    } */
}
