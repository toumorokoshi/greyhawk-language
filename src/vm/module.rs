// use super::scope;
// use std::collections::HashMap;
use super::{ops, scope};

pub struct ModuleBuilder {
    pub scope: scope::Scope,
    pub ops: Vec<ops::Op>
}

impl ModuleBuilder {
    pub fn print_ops(&self) {
        for ref op in &self.ops {
            println!("  {0}", op);
        }
        println!("FUNCTIONS: ");
        for (name, f) in &self.scope.functions {
            println!(" {0}", name);
            f.print_ops();
        }
    }
}

pub struct Module {
    pub scope_instance: scope::ScopeInstance
}
