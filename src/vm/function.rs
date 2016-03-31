use super::VM;
use super::Object;
use super::ops;
use super::scope;
use std::rc::Rc;

pub struct VMFunction {
    pub scope: scope::Scope,
    pub ops: Vec<ops::Op>,
    pub return_typ: super::types::TypeRef
}

pub enum Function {
    NativeFunction{
        function: fn(&[Object]) -> Object,
        typ: super::types::TypeRef
    },
    VMFunction(VMFunction)
}

impl Function {
    pub fn call(&self, vm: &mut VM, args: &[Object]) -> Object {
        match self {
            &Function::NativeFunction{function, ref typ} => {
                function(args)
            },
            &Function::VMFunction(ref f) => {
                let mut scopeInstance = f.scope.create_instance();
                let return_register = vm.execute_instructions(&mut scopeInstance, &f.scope, &f.ops[..]);
                Object {
                    value: scopeInstance.registers[return_register],
                    typ: f.scope.types[return_register].clone()
                }
            },
        }
    }

    pub fn return_type(&self) -> super::types::TypeRef {
        match self {
            &Function::NativeFunction{function, ref typ} => typ.clone(),
            &Function::VMFunction(ref f) => f.return_typ.clone(),
        }
    }
}
