extern crate yaml_rust;
use std::collections::BTreeMap;
use super::StatementBase;
use super::super::expressions::Expression;
use vm::scope::Scope;
use vm::Op;
use yaml_rust::Yaml;

pub struct Return {
    pub expression: Box<Expression>
}

impl StatementBase for Return {

    fn evaluate(&self, scope: &mut Scope, instructions: &mut Vec<Op>) {
        let ret_value = self.expression.generate(scope, instructions);
        instructions.push_back(Op::Return{register: ret_value.index});
    }

    fn to_yaml(&self) -> Yaml {
        let mut yaml = BTreeMap::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("return".to_string()));
        yaml.insert(Yaml::String("return".to_string()), self.expression.to_yaml());
        return Yaml::Hash(yaml);
    }
}
