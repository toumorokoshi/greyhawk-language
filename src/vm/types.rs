// use std::sync::Arc;
use std::fmt;
use std::rc::Rc;

#[derive(Clone, PartialEq)]
pub struct Type {
    name: String
}

impl fmt::Display for Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.name);
    }
}

lazy_static! {
    pub static ref ARRAY_TYPE: Rc<Type> = Rc::new(Type{name: String::from("Array")});
    pub static ref BOOL_TYPE: Rc<Type> = Rc::new(Type{name: String::from("Bool")});
    pub static ref INT_TYPE: Rc<Type> = Rc::new(Type{name: String::from("Int")});
    pub static ref FLOAT_TYPE: Rc<Type> = Rc::new(Type{name: String::from("Float")});
    pub static ref NONE_TYPE: Rc<Type> = Rc::new(Type{name: String::from("None")});
    pub static ref STRING_TYPE: Rc<Type> = Rc::new(Type{name: String::from("String")});
}

pub fn get_type_ref_from_string(symbol: &str) -> Rc<Type> {
    match symbol {
        "String" => STRING_TYPE.clone(),
        "Float" => FLOAT_TYPE.clone(),
        "Int" => INT_TYPE.clone(),
        "None" => NONE_TYPE.clone(),
        _ => NONE_TYPE.clone()
    }
}
