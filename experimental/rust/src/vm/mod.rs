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
pub use self::scope::ScopeInstance;

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

    pub fn execute_instructions(&self, scope: &mut ScopeInstance, ops: &[Op]) -> usize {
        let mut return_value = 0 as usize;
        let mut registers = scope.registers;
        for op in ops.iter() {
            match op {
                &Op::Call{func, target} => registers[target] = self.execute_function(&func).value,
                &Op::IntAdd{lhs, rhs, target} => registers[target] = registers[lhs] + registers[rhs],
                &Op::IntSub{lhs, rhs, target} => registers[target] = registers[lhs] - registers[rhs],
                &Op::IntMul{lhs, rhs, target} => registers[target] = registers[lhs] * registers[rhs],
                &Op::IntDiv{lhs, rhs, target} => registers[target] = registers[lhs] / registers[rhs],
                &Op::IntLoad{register, constant} => registers[register] = constant,
                &Op::FloatAdd{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f32, i32>(
                        mem::transmute::<i32, f32>(registers[lhs]) +
                        mem::transmute::<i32, f32>(registers[rhs]),
                    );
                },
                &Op::FloatSub{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f32, i32>(
                        mem::transmute::<i32, f32>(registers[lhs]) -
                        mem::transmute::<i32, f32>(registers[rhs]),
                    );
                },
                &Op::FloatMul{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f32, i32>(
                        mem::transmute::<i32, f32>(registers[lhs]) *
                        mem::transmute::<i32, f32>(registers[rhs]),
                    );
                },
                &Op::FloatDiv{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f32, i32>(
                        mem::transmute::<i32, f32>(registers[lhs]) /
                        mem::transmute::<i32, f32>(registers[rhs]),
                    );
                },
                &Op::FloatLoad{register, constant} => unsafe { registers[register] = mem::transmute::<f32, i32>(constant) },
                &Op::Return{register} => { return register; }
            };
        }
        return return_value;
    }

    pub fn execute_function(&mut self, func: &function::Function) -> Object {
        func.call(self)
    }
}

pub fn test() -> i32 {
    println!("this is only a test.");
    return 10;
}
