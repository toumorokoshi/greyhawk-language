use super::scope;
use vm::Op;
use vm::types;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
}

pub struct IntExpression { pub value: i32 }

impl Expression for IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::LoadInt{register: object.index, constant: self.value});
        return object;
    }
}

pub struct AddExpression {pub left: Box<Expression>, pub right: Box<Expression>}

impl Expression for AddExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> Option<scope::LocalObject> {
        let left = self.left.generate(scope, &mut instructions);
        let right = self.right.generate(scope, &mut instructions);
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::LoadInt{register: object.index, constant: self.value});
        return object;
    }
}
