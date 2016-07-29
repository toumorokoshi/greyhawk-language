// use std::sync::AArc;
use std::fmt;
use std::sync::Arc;

#[derive(Clone, PartialEq)]
pub struct _Type {
    name: String
}

pub type Type = Arc<_Type>;

impl fmt::Display for _Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.name)
    }
}

lazy_static! {
    pub static ref ARRAY_TYPE: Type = Type::new(_Type{name: String::from("Array")});
    pub static ref BOOL_TYPE: Type = Type::new(_Type{name: String::from("Bool")});
    pub static ref INT_TYPE: Type = Type::new(_Type{name: String::from("Int")});
    pub static ref FLOAT_TYPE: Type = Type::new(_Type{name: String::from("Float")});
    pub static ref NONE_TYPE: Type = Type::new(_Type{name: String::from("None")});
    pub static ref STRING_TYPE: Type = Type::new(_Type{name: String::from("String")});
}

pub fn get_type_ref_from_string(symbol: &str) -> Type {
    match symbol {
        "String" => STRING_TYPE.clone(),
        "Float" => FLOAT_TYPE.clone(),
        "Int" => INT_TYPE.clone(),
        "None" => NONE_TYPE.clone(),
        _ => NONE_TYPE.clone()
    }
}
