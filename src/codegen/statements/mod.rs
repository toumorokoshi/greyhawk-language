use std::rc::Rc;
use ast::{Statement};
use vm::{scope, types, Op};
use vm::function::{Function, VMFunction};
use super::evaluate_expr;

pub fn eval_statement(s: &Statement, scope: &mut scope::Scope, ops: &mut Vec<Op>) {
    match s {
        &Statement::FunctionDecl(ref func_decl) => {
            let mut func_scope = scope::Scope::new();
            func_scope.allocate_local(types::get_none_type());
            let mut func_ops = Vec::new();
            for s in &func_decl.statements {
                eval_statement(s, &mut func_scope, &mut func_ops);
            }
            scope.add_function(func_decl.name.clone(), Rc::new(Function::VMFunction(VMFunction{
                name: func_decl.name.clone(),
                return_typ: types::get_type_ref_from_string(&func_decl.typ),
                scope: func_scope,
                ops: func_ops
            })));
        },
        &Statement::Return(ref expr) => {
            let op = Op::Return{register: evaluate_expr(expr, scope, ops).index};
            ops.push(op);
        },
        &Statement::Expr(ref expr) => {evaluate_expr(expr, scope, ops);},
        &Statement::Declaration(ref d) => {
            let result = evaluate_expr(&(d.expression), scope, ops);
            let object = scope.add_local(&(d.name.clone()), result.typ);
            ops.push(Op::Assign{source: result.index, target: object.index});
        }
    };
}
