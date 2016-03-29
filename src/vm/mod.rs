use std::collections::HashMap;
use std::mem;

pub mod module;
pub mod function;
pub mod ops;
pub mod types;
pub mod scope;
pub mod builtins;

// for some reason, wildcards (*) don't work.
pub use self::module::Module;
pub use self::function::Function;
pub use self::function::VMFunction;
pub use self::ops::Op;
pub use self::scope::ScopeInstance;
pub use self::scope::Scope;
pub use self::builtins::print;

pub struct VM {
    pub modules: HashMap<&'static str, Module>,
}

pub struct Object {
    pub value: i32,
    pub typ: types::TypeRef,
}

impl VM{
    pub fn new() -> VM {
        return VM {modules: HashMap::new()};
    }

    pub fn execute_instructions(&mut self, scope_instance: &mut ScopeInstance, scope: &Scope, ops: &[Op]) -> usize {
        let return_value = 0 as usize;
        let mut registers = &mut scope_instance.registers;
        for op in ops.iter() {
            println!("{}", op);
            match op {
                &Op::Call{ref func, ref args, target} => {
                    let mut arg_objects = Vec::new();
                    for arg in args {
                        arg_objects.push(Object{value: registers[arg.index], typ: arg.typ.clone()});
                    }
                    registers[target] = self.execute_function(func, &arg_objects).value
                },
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

    pub fn execute_function(&mut self, func: &function::Function, args: &[Object]) -> Object {
        func.call(self, args)
    }
}
