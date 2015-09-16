use std::collections::HashMap;

pub mod module;
pub mod function;
pub mod register;
pub mod value;
pub mod ops;

// for some reason, wildcards (*) don't work.
pub use self::module::Module;
pub use self::function::Function;
pub use self::register::Register;
pub use self::ops::Op;
pub use self::value::Value;

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

    pub fn execute_instructions(&self, module: &Module, ops: &[Op]) -> Value {
        for op in ops {
            match op {
                &Op::AddInt{lhs, rhs} => println!("{}", lhs + rhs),
                &Op::ExecuteFunction{name} => self.execute_function(module, name),
            };
        }
        return Value::None;
    }

    pub fn execute_function(&self, module: &Module, name: &str) {
        match module.functions.get(name) {
            Some(function) => {
                let result = match function {
                    &Function::NativeFunction{function: nativeFunc} => nativeFunc(),
                    &Function::VMFunction{ops: ops} => {println!("not yet implemented."); Value::Int(0)},
                };
            },
            None => println!("no such function {0}", name),
        }
    }
}

pub fn test() -> Value {
    println!("this is only a test.");
    return Value::Int(10);
}
