use std::collections::HashMap;
use std::rc::Rc;
use std::mem;
use std::sync::Arc;

pub mod module;
pub mod function;
pub mod ops;
pub mod types;

pub mod scope;
pub mod builtins;
mod error;

// for some reason, wildcards (*) don't work.
pub use self::module::{Module, ModuleBuilder, dump_module};
pub use self::function::Function;
pub use self::function::VMFunction;
pub use self::ops::Op;
pub use self::scope::ScopeInstance;
pub use self::scope::Scope;
pub use self::scope::LocalObject;
pub use self::builtins::print;
pub use self::types::{get_type_ref_from_string, Type};
pub use self::error::{VMError, VMResult};

pub struct VM {
    pub module_builders: HashMap<String, Rc<ModuleBuilder>>,
    pub modules: HashMap<String, Arc<Module>>,
    strings: Vec<Rc<String>>
}

pub struct Object {
    pub value: i64,
    pub typ: Type
}

impl VM {
    pub fn new() -> VM {
        return VM {modules: HashMap::new(),
                   module_builders: HashMap::new(),
                   strings: Vec::new()};
    }

    pub fn add_string(&mut self, s: &String) -> usize {
        let index = self.strings.len();
        self.strings.push(Rc::new(s.clone()));
        index
    }

    pub fn get_string(&self, index: usize) -> Rc<String> {
        self.strings[index].clone()
    }

    pub fn execute_instructions(&mut self, scope_instance: &mut ScopeInstance, scope: &Scope, ops: &[Op]) -> usize {
        let return_value = 0 as usize;
        let mut registers = &mut scope_instance.registers;
        let mut arrays = &mut scope_instance.arrays;
        let mut i = 0;
        while i < ops.len() {
            let ref op = ops[i];
            match op {
                &Op::Assign{source, target} => {
                    registers[target] = registers[source];
                },
                &Op::ArrayCreate{target, length_source} => unsafe {
                    let length = mem::transmute::<i64, usize>(registers[length_source]);
                    let index = arrays.len();
                    arrays.push(vec![0; length]);
                    registers[target] = index as i64;
                },
                &Op::ArraySet{source, target, index_source} => unsafe {
                    let index = mem::transmute::<i64, usize>(registers[index_source]);
                    let ref mut arr = arrays[registers[target] as usize];
                    arr[index] = registers[source];
                },
                &Op::ArrayLoad{source, target, index_source} => unsafe {
                    let index = mem::transmute::<i64, usize>(registers[index_source]);
                    let ref arr = arrays[registers[source] as usize];
                    registers[target] = arr[index];
                },
                &Op::BoolNot{source, target} => {
                    registers[target] = if registers[source] != 1 { 1 } else { 0 };
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
                &Op::IntCmp{lhs, rhs, target} => registers[target] = if registers[lhs] == registers[rhs] {1} else {0},
                &Op::IntSub{lhs, rhs, target} => registers[target] = registers[lhs] - registers[rhs],
                &Op::IntMul{lhs, rhs, target} => registers[target] = registers[lhs] * registers[rhs],
                &Op::IntDiv{lhs, rhs, target} => registers[target] = registers[lhs] / registers[rhs],
                &Op::IntLoad{register, constant} => registers[register] = constant,
                &Op::IntLessEqual{lhs, rhs, target} => registers[target] = if registers[lhs] <= registers[rhs] {1} else {0},
                &Op::IntLessThan{lhs, rhs, target} => registers[target] = if registers[lhs] < registers[rhs] {1} else {0},
                &Op::FloatAdd{lhs, rhs, target} => unsafe {
                    registers[target] = mem::transmute::<f64, i64>(
                        mem::transmute::<i64, f64>(registers[lhs]) +
                        mem::transmute::<i64, f64>(registers[rhs]),
                    );
                },
                &Op::FloatCmp{lhs, rhs, target} => unsafe {
                    registers[target] = if
                        mem::transmute::<i64, f64>(registers[lhs]) ==
                        mem::transmute::<i64, f64>(registers[rhs])
                    { 1 } else { 0 };
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
                &Op::FloatLessEqual{lhs, rhs, target} => unsafe {
                    registers[target] = if
                        mem::transmute::<i64, f64>(registers[lhs]) <=
                        mem::transmute::<i64, f64>(registers[rhs])
                    { 1 } else { 0 };
                },
                &Op::FloatLessThan{lhs, rhs, target} => unsafe {
                    registers[target] = if
                        mem::transmute::<i64, f64>(registers[lhs]) <
                        mem::transmute::<i64, f64>(registers[rhs])
                    { 1 } else { 0 };
                },
                &Op::Goto{position} => {i = position - 1;},
                &Op::ModuleLoadValue{ref module_name, ref name, target} => {
                    let module = self.load_module(module_name);
                    let ref scope = module.scope;
                    let ref instance = module.scope_instance;
                    match scope.locals.get::<String>(name) {
                        Some(u) => {
                            registers[target] = instance.registers[*u];
                        },
                        None => panic!(format!("unable to retrieve variable {0} from module {1}", name, module_name))
                    }
                },
                &Op::Noop{} => {},
                // TODO: incomplete. ends up as the null pointer right now.
                &Op::StringLoad{register, ref constant} => {
                    let index = self.add_string(constant);
                    registers[register] = index as i64;
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

    pub fn build_module(&mut self, name: &String, mb: &ModuleBuilder) -> Arc<Module> {
        let mut scope_instance = mb.scope.create_instance();
        self.execute_instructions(&mut scope_instance, &mb.scope, &mb.ops[..]);
        let module = Arc::new(Module{
            scope: mb.scope.clone(),
            scope_instance: scope_instance
        });
        self.modules.insert(name.clone(), module.clone());
        module
    }

    pub fn load_module(&mut self, name: &String) -> Arc<Module> {
        if let Some(ref m) = self.modules.get(name) {
            return (*m).clone();
        }

        let mut module_builder: Option<Rc<ModuleBuilder>> = None;
        if let Some(ref mb) = self.module_builders.get(name) {
            module_builder = Some((*mb).clone());
        }
        if let Some(mb) = module_builder {
            return self.build_module(name, &mb);
        }
        match module::find_module_from_classpath(self, name) {
            Ok(module) => {
                let module_wrapped = Arc::new(module);
                self.modules.insert(name.clone(), module_wrapped.clone());
                return module_wrapped;
            },
            Err(msg) => { panic!("unable to find module."); }
        }
    }
}
