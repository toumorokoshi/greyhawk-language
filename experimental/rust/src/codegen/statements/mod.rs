extern crate yaml_rust;
use ast::Statement;
mod function_declaration;
mod return_declaration;
use vm::Op;
use vm::scope;
use yaml_rust::Yaml;
use super::expressions::Expression;
pub use self::function_declaration::FunctionDeclaration;
pub use self::return_declaration::Return;

pub evaluate_statement(stmt: Statement, scope: &mut scope::Scope, instructions: &mut Vec<Op>) {
    match stmt {
    }
}

pub enum Statement {
    Func(FunctionDeclaration),
    Return(Return),
    Expr(Expression),
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


pub trait StatementBase {
    fn evaluate(&self, scope: &mut scope::Scope, instructions: &mut Vec<Op>);
    fn to_yaml(&self) -> Yaml;
}
