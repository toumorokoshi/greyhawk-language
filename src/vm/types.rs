// use std::sync::AArc;
use std::fmt;
use std::sync::{Arc, Mutex};
use std::collections::HashMap;

#[derive(Clone, PartialEq, Eq, Hash)]
pub struct _Type {
    pub name: String,
    pub sub_types: Vec<Type>,
}

pub type Type = Arc<_Type>;

impl fmt::Display for _Type {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.name)
    }
}

lazy_static! {
    pub static ref BOOL_TYPE: Type = Type::new(_Type{name: String::from("Bool"), sub_types: Vec::new()});
    pub static ref INT_TYPE: Type = Type::new(_Type{name: String::from("Int"), sub_types: Vec::new()});
    pub static ref FLOAT_TYPE: Type = Type::new(_Type{name: String::from("Float"), sub_types: Vec::new()});
    pub static ref NONE_TYPE: Type = Type::new(_Type{name: String::from("None"), sub_types: Vec::new()});
    pub static ref STRING_TYPE: Type = Type::new(_Type{name: String::from("String"), sub_types: Vec::new()});
    pub static ref ARRAY_CACHE: Mutex<HashMap<Type, Type>> = Mutex::new(HashMap::new());
}

pub fn get_array_type(array_type: Type) -> Type {
    match ARRAY_CACHE.lock().unwrap().get(&array_type) {
        Some(t) => t.clone(),
        None => {
            let new_type = Type::new(_Type{name: String::from("Array"), sub_types: vec![array_type.clone()]});
            ARRAY_CACHE.lock().unwrap().insert(
                array_type.clone(), new_type.clone()
            );
            new_type
        }
    }
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
