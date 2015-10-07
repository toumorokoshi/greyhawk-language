use std::collections::HashMap;
use vm::types::TypeRef;

pub struct LocalObject {
    pub index: usize,
    pub typ: TypeRef
}

impl Clone for LocalObject {
    fn clone(&self) -> LocalObject {
        return LocalObject {
            index: self.index,
            typ: match self.typ {
                TypeRef::Heap(ref a) => TypeRef::Heap(a.clone()),
                TypeRef::Static(t) => TypeRef::Static(t),
            }
        };
    }
}

pub struct Scope {
    pub locals: HashMap<&'static str, LocalObject>,
    pub local_count: usize,
}

impl Scope {
    pub fn new() -> Scope {
        return Scope{locals: HashMap::new(), local_count: 0};
    }

    pub fn add_local(&mut self, name: &'static str, typ: TypeRef) -> LocalObject {
        let object = self.allocate_local(typ);
        self.locals.insert(name, object.clone());
        return object;
    }

    pub fn allocate_local(&mut self, typ: TypeRef) -> LocalObject {
        let object = LocalObject{
            index: self.local_count, typ: typ
        };
        self.local_count += 1;
        return object;
    }
}
