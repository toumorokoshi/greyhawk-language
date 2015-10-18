use std::collections::HashMap;
use super::types;
use super::types::TypeRef;

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
    pub locals: HashMap<&'static str, usize>,
    pub types: Vec<types::TypeRef>
}

impl Scope {
    pub fn new() -> Scope {
        return Scope{locals: HashMap::new(), types: Vec::new()};
    }

    pub fn add_local(&mut self, name: &'static str, typ: TypeRef) -> LocalObject {
        let object = self.allocate_local(typ);
        self.locals.insert(name, object.index);
        return object;
    }

    pub fn allocate_local(&mut self, typ: TypeRef) -> LocalObject {
        self.types.push(typ.clone());
        return LocalObject {
            index: self.types.len() - 1,
            typ: typ
        };
    }

    pub fn local_count(&self) -> usize {
        return self.types.len();
    }
}
