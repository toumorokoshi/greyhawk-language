extern crate yaml_rust;
use super::Expression;
use super::super::Statement;
use vm::Op;
use vm::types;
use vm::scope;
use yaml_rust::Yaml;


pub struct IntExpression { pub value: i64 }

impl IntExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::IntLoad{register: object.index, constant: self.value});
        return object;
    }
}

impl Expression for IntExpression {
    fn generate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        self.generate(scope, instructions)
    }

    fn to_yaml(&self) -> Yaml {
        return Yaml::Integer(self.value as i64);
    }
}
