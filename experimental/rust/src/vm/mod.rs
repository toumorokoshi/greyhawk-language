use std::collections::HashMap;

pub mod module;
pub mod function;
pub mod ops;

// for some reason, wildcards (*) don't work.
pub use self::module::Module;
pub use self::function::Function;
pub use self::ops::Op;

pub struct VM {
    pub modules: HashMap<&'static str, Module>,
}

impl VM {
    pub fn new() -> VM {

        let mut main_module = Module{ functions: HashMap::new()};
        main_module.functions.insert(
            "foo", Function::NativeFunction{function: test}
        );

        let mut modules = HashMap::new();
        modules.insert("main", main_module);

        return VM {modules: modules};
    }

    pub fn execute_instructions(&self, module: &Module,
                                registers: &mut &[i32], ops: &[Op]) -> i32 {
        for op in ops {
            match op {
                &Op::AddConstantInt{register, constant} => {
                    registers[register as usize] =
                        registers[register as usize] + constant;
                },
                &Op::ExecuteFunction{name} => self.execute_function(module, name),
            };
        }
        return 0;
    }

    pub fn execute_function(&self, module: &Module, name: &str) {
        match module.functions.get(name) {
            Some(function) => {
                let result = match function {
                    &Function::NativeFunction{function: nativeFunc} => nativeFunc(),
                    &Function::VMFunction{register_count, ops} => {println!("not yet implemented."); 0},
                };
            },
            None => println!("no such function {0}", name),
        }
    }
}

pub fn test() -> i32 {
    println!("this is only a test.");
    return 10;
}
