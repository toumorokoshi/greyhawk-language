use std::collections::HashMap;
use vm::types::Type;

pub struct LocalObject {
    index: usize,
    typ: Type,
}

pub struct Scope {
    pub locals: HashMap<&'static str, Box<LocalObject>>,
    pub local_count: usize,
}

impl Scope {
    pub fn add_local(&mut self, name: &'static str, typ: Type) {
        let index = self.local_count;
        let object = Box::new(LocalObject{
            index: self.local_count + 1, typ: typ
        });
        self.local_count += 1;
        self.locals.insert(name, object);
    }
}
