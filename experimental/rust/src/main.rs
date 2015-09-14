mod vm;

use std::io;

fn main () {
    println!("Greyhawk 0.0.3");
    let ops = &[vm::Op::AddInt { lhs: 0, rhs: 1 }];
    vm::execute_instructions(ops);
    /* loop {
        println!(">> ");
        let mut guess = String::new();
        io::stdin().read_line(&mut guess)
            .ok()
            .expect("Failed to read line");
        println!("received input: {}", guess);
    } */
}
