use super::peg_grammar;
use super::vm;

fn build_module(vm_instance: vm::VM, body: &String) {
    match peg_grammar::module(&body) {
        Ok(statement_list) => {
            codegen::generate_ops(statements: statement_list) {
            }
        }
    }
}
