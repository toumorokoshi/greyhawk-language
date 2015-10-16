use std::sync::Arc;

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

pub fn get_int_type() -> TypeRef {
    return TypeRef::Static(&IntType);
}

pub fn get_float_type() -> TypeRef {
    return TypeRef::Static(&FloatType);
}
