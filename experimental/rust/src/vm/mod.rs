use std::collections::HashMap;

pub mod module;
pub mod function;
pub mod register;
pub mod ops;

// for some reason, wildcards (*) don't work.
pub use self::module::Module;
pub use self::function::Function;
pub use self::register::Register;
pub use self::ops::Op;

pub struct VM {
    pub modules: HashMap<&'static str, Module>,
}

impl VM {
    pub fn new() -> VM {
        let mut modules = HashMap::new();
        modules.insert("main", Module{ functions: HashMap::new()});
        return VM {modules: modules};
    }

    pub fn execute_instructions(&self, module: &Module, ops: &[Op]) {
        for op in ops {
            match op {
                &Op::AddInt{lhs, rhs} => println!("{}", lhs + rhs),
                &Op::ExecuteFunction{name} => self.execute_function(module, name),
            };
        }
    }

    pub fn execute_function(&self, module: &Module, name: &str) {
        println!("execute a function!");
    }
}
