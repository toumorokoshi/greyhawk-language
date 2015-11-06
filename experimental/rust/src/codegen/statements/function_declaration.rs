extern crate yaml_rust;
use yaml_rust::Yaml;
use std::collections::BTreeMap;
use std::rc::Rc;
use super::Statement;
use vm::function::VMFunction;
use vm::function::Function;
use vm::scope::Scope;
use vm::Op;

pub struct FunctionDeclaration {
    pub name: String,
    pub statements: Vec<Box<Statement>>
}

impl Statement for FunctionDeclaration {

    fn evaluate(&self, scope: &mut Scope, instructions: &mut Vec<Op>) {
        let mut function_scope = Scope::new();
        let mut ops = Vec::new();
        for statement in self.statements {
            statement.evaluate(&mut function_scope, &mut ops);
        }
        scope.add_function(self.name.clone(), Rc::new(Function::VMFunction(VMFunction {
            scope: function_scope,
            ops: ops
        })));
    }

    fn to_yaml(&self) -> Yaml {
        let mut yaml = BTreeMap::new();
        yaml.insert(Yaml::String("type".to_string()), Yaml::String("call".to_string()));
        return Yaml::Hash(yaml);
    }
}
