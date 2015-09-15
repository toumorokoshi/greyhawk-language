use super::function;
use std::collections::HashMap;

pub struct Module {
    pub functions: HashMap<&'static str, function::Function>,
}
