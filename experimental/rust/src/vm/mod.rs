use std::collections::HashMap;

pub mod module;
pub mod function;
pub mod ops;
pub mod types;

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
                                registers: &mut [i32], ops: &Box<[Op]>) -> i32 {
        for op in ops.iter() {
            match op {
                &Op::AddConstantInt{register, constant} =>
                    registers[register] = registers[register] + constant,

                &Op::AddInt{lhs, rhs} =>
                    registers[lhs] = registers[lhs] + registers[rhs],
            };
        }
        return 0;
    }

    pub fn execute_function(&self, module: &Module, func: &function::Function) -> i32 {
        return match func {
            &Function::NativeFunction{function: nativeFunc} => {
                return nativeFunc();
            },
            &Function::VMFunction{register_count, ref ops} => {
                let mut registers = vec![0; register_count];
                return self.execute_instructions(module, &mut registers, ops);
            },
        };
    }
}

pub fn test() -> i32 {
    println!("this is only a test.");
    return 10;
}
