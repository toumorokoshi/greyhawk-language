extern crate yaml_rust;
pub mod statements;
pub mod expressions;
pub mod yaml;
use super::vm;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;
use std::rc::Rc;

pub use self::statements::StatementBase;
pub use self::statements::FunctionDeclaration;
pub use self::expressions::Expression;
pub use self::expressions::CallExpression;
pub use self::expressions::FloatExpression;
pub use self::expressions::IntExpression;
pub use self::expressions::BinOpExpression;
pub use self::yaml::to_yaml;

pub fn generate_ops(statements: &Vec<Statement>) -> vm::Function {
    let mut ops: Vec<vm::ops::Op> = Vec::new();
    let mut scope = vm::scope::Scope::new();
    for statement in statements {
        statement.evaluate(&mut scope, &mut ops);
    }
    return vm::Function::VMFunction(vm::VMFunction {
        scope: scope,
        ops: ops
    });
}

pub enum Statement {
    StatementBase(Box<StatementBase>),
    Expression(Box<Expression>),
}

impl Statement {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>) {
        match self {
            &Statement::StatementBase(ref statement) => statement.evaluate(scope, instructions),
            &Statement::Expression(ref expr) => { expr.generate(scope, instructions); }
        };
    }

    fn to_yaml(&self) -> Yaml {
        match self {
            &Statement::StatementBase(ref statement) => statement.to_yaml(),
            &Statement::Expression(ref expr) => expr.to_yaml()
        }
    }
}
