use std::fmt;
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

pub struct ScopeInstance {
    pub registers: Vec<i32>
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

    pub fn create_instance(&self) -> ScopeInstance {
        return ScopeInstance{registers: vec![0; self.local_count()]};
    }
}

impl fmt::Display for Scope {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "Locals:\n");
        for (key, value) in &self.locals {
            write!(f, "  {}: {}\n", key, value);
        }
        write!(f, "Types:\n");
        for typ in &self.types {
            write!(f, "  {}\n", typ);
        }
        return write!(f, "");
    }
}
