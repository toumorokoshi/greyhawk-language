use std::collections::HashMap;
use vm::types::Type;

pub struct LocalObject {
    pub index: usize,
    pub typ: Box<Type>,
}

pub struct Scope {
    pub locals: HashMap<&'static str, Box<LocalObject>>,
    pub local_count: usize,
}

impl Scope {
    pub fn add_local(&mut self, name: &'static str, typ: Box<Type>) -> Box<LocalObject> {
        let object = self.allocate_local(typ);
        self.locals.insert(name, object);
        return object;
    }

    pub fn allocate_local(&mut self, typ: Box<Type>) -> Box<LocalObject> {
        let object = Box::new(LocalObject{
            index: self.local_count + 1, typ: typ
        });
        self.local_count += 1;
        return object;
    }
}
