use std::collections::HashMap;
use std::mem;

pub mod module;
pub mod function;
pub mod ops;
pub mod types;
pub mod scope;

// for some reason, wildcards (*) don't work.
pub use self::module::Module;
pub use self::function::Function;
pub use self::function::VMFunction;
pub use self::ops::Op;

pub struct VM {
    pub modules: HashMap<&'static str, Module>,
}

pub struct Object {
    pub value: i32,
    pub typ: types::TypeRef,
}

impl VM{
    pub fn new() -> VM {

        let mut main_module = Module{ functions: HashMap::new()};
        main_module.functions.insert(
            "foo", Function::NativeFunction{function: test, typ: types::get_int_type()}
        );

        let mut modules = HashMap::new();
        modules.insert("main", main_module);

        return VM {modules: modules};
    }

    pub fn execute_instructions(&self, module: &Module,
                                registers: &mut [i32], ops: &[Op]) -> usize {
        let mut return_value = 0 as usize;
        for op in ops.iter() {
            match op {

                &Op::IntAdd{lhs, rhs, target} =>
                    registers[target] = registers[lhs] + registers[rhs],

                &Op::IntLoad{register, constant} =>
                    registers[register] = constant,

                &Op::FloatAdd{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f32, i32>(
                        mem::transmute::<i32, f32>(registers[lhs]) +
                        mem::transmute::<i32, f32>(registers[rhs]),
                    );
                },

                &Op::FloatLoad{register, constant} => unsafe {
                    registers[register] = mem::transmute::<f32, i32>(constant);
                },

                &Op::Return{register} => {
                    return register;
                }
            };
        }
        return return_value;
    }

    pub fn execute_function(&self, module: &Module, func: &function::Function) -> Object {
        return match func {
            &Function::NativeFunction{function, ref typ} => {
                return Object{
                    value: function(),
                    typ: typ.clone()
                };
            },
            &Function::VMFunction(ref f) => {
                let mut registers = vec![0; f.scope.local_count()];
                let return_register = self.execute_instructions(module, &mut registers, &f.ops[..]);
                return Object{
                    value: registers[return_register],
                    typ: f.scope.types[return_register].clone()
                };
            },
        };
    }
}

pub fn test() -> i32 {
    println!("this is only a test.");
    return 10;
}
