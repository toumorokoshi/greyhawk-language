use super::scope;
use vm::Op;
use vm::types;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
}

pub struct IntExpression { pub value: i32 }

impl Expression for IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_float_type());
        instructions.push(Op::IntLoad{register: object.index, constant: self.value});
        return object;
    }
}

pub struct FloatExpression { pub value: f32 }

impl Expression for FloatExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::FloatLoad{register: object.index, constant: self.value});
        return object;
    }
}

pub struct AddExpression {pub left: Box<Expression>, pub right: Box<Expression>}

impl Expression for AddExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let left = self.left.generate(scope, instructions);
        let right = self.right.generate(scope, instructions);
        let object = scope.allocate_local(types::get_int_type());
        if (left.typ == types::get_float_type()) {
            instructions.push(Op::FloatAdd{lhs: left.index, rhs: right.index, target: object.index});
        } else {
            instructions.push(Op::IntAdd{lhs: left.index, rhs: right.index, target: object.index});
        }
        return object;
    }
}
