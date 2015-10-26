extern crate yaml_rust;
use lexer::token::TokenType;
use vm::Op;
use vm::types;
use vm::scope;
use yaml_rust::Yaml;
use yaml_rust::yaml;

pub trait Expression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject;
    fn to_yaml(&self) -> Yaml;
}

pub struct IntExpression { pub value: i32 }

impl Expression for IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::IntLoad{register: object.index, constant: self.value});
        return object;
    }

    fn to_yaml(&self) -> Yaml {
        return Yaml::Integer(self.value as i64);
    }
}

pub struct FloatExpression { pub value: f32 }

impl Expression for FloatExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_float_type());
        instructions.push(Op::FloatLoad{register: object.index, constant: self.value});
        return object;
    }

    fn to_yaml(&self) -> Yaml {
        return Yaml::Real(self.value.to_string());
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

    fn to_yaml(&self) -> Yaml {
        let mut yaml = yaml::Hash::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("binop".to_string()));
        yaml.insert(Yaml::String("op".to_string()), Yaml::String(self.op.to_string()));
        yaml.insert(Yaml::String("left".to_string()), self.left.to_yaml());
        yaml.insert(Yaml::String("right".to_string()), self.left.to_yaml());
        return Yaml::Hash(yaml);
    }
}

pub struct CallExpression {pub name: String}

impl Expression for CallExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        return scope.allocate_local(types::get_int_type());
    }

    fn to_yaml(&self) -> Yaml {
        let mut yaml = yaml::Hash::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
        yaml.insert(Yaml::String("func_name".to_string()), Yaml::String(self.name.clone()));
        return Yaml::Hash(yaml);
    }
}
