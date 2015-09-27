use std::collections::HashMap;
use vm::types::TypeRef;
use std::rc::Rc;

pub struct LocalObject {
    pub index: usize,
    pub typ: TypeRef
}

pub struct Scope {
    pub locals: HashMap<&'static str, Rc<LocalObject>>,
    pub local_count: usize,
}

impl Scope {
    pub fn add_local(&mut self, name: &'static str, typ: TypeRef) -> Rc<LocalObject> {
        let object = self.allocate_local(typ);
        self.locals.insert(name, object.clone());
        return object;
    }

    pub fn allocate_local(&mut self, typ: TypeRef) -> Rc<LocalObject> {
        let object = Rc::new(LocalObject{
            index: self.local_count + 1, typ: typ
        });
        self.local_count += 1;
        return object;
    }
}
