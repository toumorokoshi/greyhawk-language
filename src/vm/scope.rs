use std::fmt;
use std::collections::HashMap;
use std::sync::Arc;
use std::rc::Rc;
use super::types;
use super::types::Type;
use super::Function;
use super::builtins::print;

pub struct LocalObject {
    pub index: usize,
    pub typ: Type
}

impl Clone for LocalObject {
    fn clone(&self) -> LocalObject {
        return LocalObject {
            index: self.index,
            typ: self.typ.clone(),
        };
    }
}

pub struct Scope {
    pub parent: Option<Arc<Scope>>,
    pub locals: HashMap<String, usize>,
    pub functions: HashMap<String, Rc<Function>>,
    pub types: Vec<Type>
}

impl Scope {
    pub fn new(scope: Option<Arc<Scope>>) -> Scope {
        return Scope{
            parent: scope.clone(),
            functions: HashMap::new(),
            locals: HashMap::new(),
            types: Vec::new()
        };
    }

    pub fn get_local(&self, name: &String) -> Option<LocalObject> {
        match self.locals.get(name) {
            None => None,
            Some(index) => {
                Some(LocalObject{index: index.clone(), typ: self.types[index.clone()].clone()})
            }
        }
    }

    pub fn add_local(&mut self, name: &String, typ: Type) -> LocalObject {
        if let Some(_) = self.locals.get(name) {
            panic!(format!("cannot redeclare previously declared local {0}", name));
        }
        let object = self.allocate_local(typ);
        self.locals.insert(name.clone(), object.index);
        return object;
    }

    pub fn add_function(&mut self, name: String, function: Rc<Function>) {
        self.functions.insert(name, function);
    }

    pub fn allocate_local(&mut self, typ: Type) -> LocalObject {
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
        return ScopeInstance{
            registers: vec![0; self.local_count()],
            arrays: Vec::new()
        };
    }

    pub fn get_function(&self, name: &String) -> Rc<Function> {
        if let Some(func) = self.functions.get(name) {
            return func.clone();
        } else {
            return Rc::new(Function::NativeFunction {
                name: String::from("print"),
                function: print,
                typ: types::NONE_TYPE.clone(),
            });
        }
    }
}

impl fmt::Display for Scope {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        try!(write!(f, "Locals:\n"));
        for (key, value) in &self.locals {
            try!(write!(f, "  {}: {}\n", key, value));
        }
        try!(write!(f, "Types:\n"));
        for typ in &self.types {
            try!(write!(f, "  {}\n", typ));
        }
        return write!(f, "");
    }
}

pub struct ScopeInstance {
    pub registers: Vec<i64>,
    // arrays are stored within the
    // scope instance, to allow the
    // vector to continue to exist
    // even when a reference to it is removed
    // from unsafe memory casts.
    pub arrays: Vec<Vec<i64>>
}
