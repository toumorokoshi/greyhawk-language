use lexer::token::TokenType;
use vm::Op;
use vm::types;
use vm::scope;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
}

pub struct IntExpression { pub value: i32 }

impl Expression for IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::IntLoad{register: object.index, constant: self.value});
        return object;
    }
}

pub struct FloatExpression { pub value: f32 }

impl Expression for FloatExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_float_type());
        instructions.push(Op::FloatLoad{register: object.index, constant: self.value});
        return object;
    }
}

pub struct BinOpExpression {pub op: TokenType,  pub left: Box<Expression>, pub right: Box<Expression>}

impl Expression for BinOpExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let left = self.left.generate(scope, instructions);
        let right = self.right.generate(scope, instructions);
        return if (left.typ == types::get_float_type()) {
            let object = scope.allocate_local(types::get_float_type());
            match self.op {
                TokenType::Plus => instructions.push(Op::FloatAdd{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Sub => instructions.push(Op::FloatSub{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Mul => instructions.push(Op::FloatMul{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Div => instructions.push(Op::FloatDiv{lhs: left.index, rhs: right.index, target: object.index}),
                // TODO: this should be validated when creating the
                // expression, rather that during evaluation.
                _ => {},
            };
            object
        } else {
            let object = scope.allocate_local(types::get_int_type());
            match self.op {
                TokenType::Plus => instructions.push(Op::IntAdd{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Sub => instructions.push(Op::IntSub{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Mul => instructions.push(Op::IntMul{lhs: left.index, rhs: right.index, target: object.index}),
                TokenType::Div => instructions.push(Op::IntDiv{lhs: left.index, rhs: right.index, target: object.index}),
                // TODO: this should be validated when creating the
                // expression, rather that during evaluation.
                _ => {},
            };
            object
        };
    }
}

pub struct CallExpression {pub name: String}

impl Expression for CallExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        return scope.allocate_local(types::get_int_type());
    }
}
