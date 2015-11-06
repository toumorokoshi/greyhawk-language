extern crate yaml_rust;
use super::Expression;
use super::super::Statement;
use vm::Op;
use vm::types;
use vm::scope;
use std::rc::Rc;
use std::collections::BTreeMap;
use yaml_rust::Yaml;

pub struct CallExpression {pub name: String, pub arg: Box<Expression>}

impl Expression for CallExpression {
    fn generate (&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) -> scope::LocalObject {
        let mut args = Vec::new();
        args.push(self.arg.generate(scope, instructions));

        let ret_object = scope.allocate_local(types::get_int_type());
        instructions.push(Op::Call {
            func: scope.get_function(self.name.clone()), args: args, target: ret_object.index
        });
        return ret_object;
    }
}

impl Statement for CallExpression {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) {
        self.generate(scope, instructions);
    }

    fn to_yaml(&self) -> Yaml {
        let mut yaml = BTreeMap::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
        yaml.insert(Yaml::String("func_name".to_string()), Yaml::String(self.name.clone()));
        return Yaml::Hash(yaml);
    }
}
