use super::scope;
use vm::Op;
use vm::types;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> Option<scope::LocalObject>;
}

pub struct IntExpression { value: i32 }

impl Expression for IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> Option<scope::LocalObject> {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::LoadInt{register: object.index, constant: self.value});
        return None;
    }
}
