use std::sync::Arc;
use std::fmt;

pub struct Type {
    name: &'static str
}

pub enum TypeRef {
    // we use Arcs because it's thread-safe,
    // and type references are shared across
    // processes.
    Heap(Arc<Type>),
    Static(&'static Type),
}

impl TypeRef {
    fn name(&self) -> &str {
        return match self {
            &TypeRef::Heap(ref t) => t.name,
            &TypeRef::Static(ref t) => t.name
        }
    }
}

impl Clone for TypeRef {
    fn clone(&self) -> TypeRef {
        return match self {
            &TypeRef::Heap(ref a) => TypeRef::Heap(a.clone()),
            &TypeRef::Static(t) => TypeRef::Static(t),
        };
    }

}


impl fmt::Display for TypeRef {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", match self {
            &TypeRef::Heap(ref t) => t.name,
            &TypeRef::Static(t) => t.name
        })
    }
}

impl PartialEq<TypeRef> for TypeRef {
    fn eq(&self, other: &TypeRef) -> bool {
        return self.name() == other.name();
    }
    fn ne(&self, other: &TypeRef) -> bool {
        return self.name() != other.name();
    }
}

pub static IntType: Type = Type {name: "Int"};
pub static FloatType: Type = Type {name: "Float"};
pub static NoneType: Type = Type {name: "None"};
pub static StringType: Type = Type {name: "String"};

pub fn get_int_type() -> TypeRef {
    return TypeRef::Static(&IntType);
}

pub fn get_float_type() -> TypeRef {
    return TypeRef::Static(&FloatType);
}

pub fn get_none_type() -> TypeRef {
    return TypeRef::Static(&NoneType);
}

pub fn get_string_type() -> TypeRef {
    return TypeRef::Static(&StringType);
}

pub fn get_type_ref_from_string(symbol: &str) -> TypeRef {
    match symbol {
        "String" => get_string_type(),
        "Float" => get_float_type(),
        "Int" => get_int_type(),
        "None" => get_none_type(),
        _ => get_none_type(),
    }
}
