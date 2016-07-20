use std::collections::HashMap;
use std::rc::Rc;
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
pub use self::scope::LocalObject;
pub use self::builtins::print;
pub use self::types::get_type_ref_from_string;

pub struct VM {
    pub modules: HashMap<&'static str, Module>,
}

pub struct Object {
    pub value: i64,
    pub typ: types::TypeRef,
}

impl VM{
    pub fn new() -> VM {
        return VM {modules: HashMap::new()};
    }

    pub fn execute_instructions(&mut self, scope_instance: &mut ScopeInstance, scope: &Scope, ops: &[Op]) -> usize {
        let return_value = 0 as usize;
        let mut registers = &mut scope_instance.registers;
        let mut i = 0;
        while i < ops.len() {
            let ref op = ops[i];
            match op {
                &Op::Assign{source, target} => {
                    registers[target] = registers[source];
                },
                &Op::BoolNot{source, target} => {
                    registers[target] = registers[source] != 1 ? 0 : 1;
                },
                &Op::Branch{condition, if_false} => {
                    if registers[condition] == 0 {
                        // -1 to allow an increment at the end of the
                        // function.
                        i = if_false - 1;
                    }
                },
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
                    registers[target] = mem::transmute::<f64, i64>(
                        mem::transmute::<i64, f64>(registers[lhs]) +
                        mem::transmute::<i64, f64>(registers[rhs]),
                    );
                },
                &Op::FloatSub{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f64, i64>(
                        mem::transmute::<i64, f64>(registers[lhs]) -
                        mem::transmute::<i64, f64>(registers[rhs]),
                    );
                },
                &Op::FloatMul{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f64, i64>(
                        mem::transmute::<i64, f64>(registers[lhs]) *
                        mem::transmute::<i64, f64>(registers[rhs]),
                    );
                },
                &Op::FloatDiv{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f64, i64>(
                        mem::transmute::<i64, f64>(registers[lhs]) /
                        mem::transmute::<i64, f64>(registers[rhs]),
                    );
                },
                &Op::FloatLoad{register, constant} => unsafe {
                    registers[register] = mem::transmute::<f64, i64>(constant)
                },
                &Op::Goto{position} => {
                    i = position - 1;
                },
                &Op::Noop{} => {},
                // TODO: incomplete. ends up as the null pointer right now.
                &Op::StringLoad{register, ref constant} => unsafe {
                    registers[register] = mem::transmute::<Rc<String>, i64>(constant.clone());
                },
                &Op::Return{register} => { return register; },
            };
            i +=1;
        }
        return return_value;
    }

    pub fn execute_function(&mut self, func: &function::Function, args: &[Object]) -> Object {
        func.call(self, args)
    }
}
