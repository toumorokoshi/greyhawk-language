use super::{Object, VM};
use super::types;
use std::mem;

/*
pub fn get_builtin_module() -> Module {
    let mut scope = Scope::new();
    let instance = scope.create_instance();
    return Module {
        scope: scope,
        scope_instance: instance
    };
}
*/

pub fn print(vm: &VM, args: &[Object]) -> Object {
    if args.len() > 0 {
        let object = &args[0];
        if object.typ == *types::INT_TYPE {
            println!("{}", object.value);
        } else if object.typ == *types::FLOAT_TYPE {
            unsafe {
                println!("{}", mem::transmute::<i64, f64>(object.value));
            }
        } else if object.typ == *types::STRING_TYPE {
            let value = vm.get_string(object.value as usize);
            println!("{}", value);
        }
    }
    return Object {
        value: 0,
        typ: types::NONE_TYPE.clone()
    };
}
