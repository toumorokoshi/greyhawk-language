use super::module::Module;
use super::scope::Scope;
use super::Object;
use super::types;
use std::mem;

pub fn getBuiltinModule() -> Module {
    let mut scope = Scope::new();
    let instance = scope.create_instance();
    return Module {
        scope: scope,
        scope_instance: instance
    };
}

pub fn print(args: &[Object]) -> Object {
    let object = &args[0];
    if (object.typ == types::get_int_type()) {
        println!("int: {}", object.value);
    } else if (object.typ == types::get_float_type()) {
        unsafe {
            println!("float: {}", mem::transmute::<i32, f32>(object.value));
        }
    }
    return Object {
        value: 0,
        typ: types::get_none_type()
    };
}
