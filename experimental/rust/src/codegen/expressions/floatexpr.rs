extern crate yaml_rust;
use super::Expression;
use super::super::Statement;
use vm::Op;
use vm::types;
use vm::scope;
use yaml_rust::Yaml;

pub struct FloatExpression { pub value: f32 }

impl Expression for FloatExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let object = scope.allocate_local(types::get_float_type());
        instructions.push(Op::FloatLoad{register: object.index, constant: self.value});
        return object;
    }

}

impl Statement for FloatExpression {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) {
        self.generate(scope, instructions);
    }

    fn to_yaml(&self) -> Yaml {
        return Yaml::Real(self.value.to_string());
    }
}
