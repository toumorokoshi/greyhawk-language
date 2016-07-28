use super::VM;
use super::Object;
use super::ops;
use super::scope;
use super::types;
use super::types::Type;

pub struct VMFunction {
    pub name: String,
    pub argument_names: Vec<String>,
    pub scope: scope::Scope,
    pub ops: Vec<ops::Op>,
    pub return_typ: Rc<Type>
}

pub enum Function {
    NativeFunction{
        name: String,
        function: fn(&[Object]) -> Object,
        typ: Rc<Type>
    },
    VMFunction(VMFunction)
}

impl Function {

    pub fn name(&self) -> String {
        match self {
            &Function::NativeFunction{ref name, function: _, typ: _} => name.clone(),
            &Function::VMFunction(ref f) => f.name.clone()
        }
    }

    pub fn call(&self, vm: &mut VM, args: &[Object]) -> Object {
        match self {
            &Function::NativeFunction{name: _, function, typ: _} => {
                function(args)
            },
            &Function::VMFunction(ref f) => {
                let mut scope_instance = f.scope.create_instance();
                for i in 0..(f.argument_names.len()) {
                    match f.scope.get_local(&f.argument_names[i]) {
                        Some(o) => {
                            scope_instance.registers[o.index] = args[i].value;
                        },
                        _ => {}
                    }
                }
                let return_register = vm.execute_instructions(&mut scope_instance, &f.scope, &f.ops[..]);
                if scope_instance.registers.len() == 0 {
                    Object {
                        value: 0,
                        typ: types::NONE_TYPE.clone()
                    }
                } else {
                    Object {
                        value: scope_instance.registers[return_register],
                        typ: f.scope.types[return_register].clone()
                    }
                }
            },
        }
    }

    pub fn return_type(&self) -> Rc<Type> {
        match self {
            &Function::NativeFunction{name: _, function: _, ref typ} => typ.clone(),
            &Function::VMFunction(ref f) => f.return_typ.clone(),
        }
    }

    /* pub fn print_ops(&self) {
        match self {
            &Function::NativeFunction{name: _, function: _, typ: _} => println!("  native function"),
            &Function::VMFunction(ref f) => {
                for ref op in &f.ops {
                    println!("  {0}", op);
                }
            }
        }
    } */
}
