extern crate yaml_rust;
use super::Expression;
use super::super::Statement;
use lexer::token::TokenType;
use vm::Op;
use vm::types;
use vm::scope;
use std::collections::BTreeMap;
use yaml_rust::Yaml;

pub struct BinOpExpression {
    pub op: TokenType,
    pub left: Box<Expression>,
    pub right: Box<Expression>
}

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


impl Statement for BinOpExpression {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) {
        self.generate(scope, instructions);
    }

    fn to_yaml(&self) -> Yaml {
        let mut yaml = BTreeMap::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("binop".to_string()));
        yaml.insert(Yaml::String("op".to_string()), Yaml::String(self.op.to_string()));
        yaml.insert(Yaml::String("left".to_string()), self.left.to_yaml());
        yaml.insert(Yaml::String("right".to_string()), self.left.to_yaml());
        return Yaml::Hash(yaml);
    }
}
