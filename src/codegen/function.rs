use ast::{Statement, FunctionDecl};
use vm::{VM, Function, VMFunction, types};
use super::{CGResult, gen_block};


pub fn generate_function(vm: &mut VM, statements: &Vec<Box<Statement>>) -> CGResult<Function> {
    let block = try!(gen_block(vm, statements));
    Ok(Function::VMFunction(VMFunction {
        name: String::from("__main__"),
        argument_names: vec![],
        scope: block.scope,
        ops: block.ops,
        return_typ: types::NONE_TYPE.clone(),
    }))
}

pub fn gen_function(vm: &mut VM, parent: Option<Arc<Scope>>, func_decl: &FunctionDecl) -> CGResult<Function> {
    let mut func_scope = Scope::new(parent);
    // allocate return type
    func_scope.allocate_local(types::NONE_TYPE.clone());
    let mut argument_names = Vec::new();
    for ref a in &(func_decl.arguments) {
        let typ = get_type_ref_from_string(&a.typ);
        func_scope.add_local(&a.name, typ);
        argument_names.push(a.name.clone());
    }
    let mut func_context = Context{
        block: Block {
            ops: vec![],
            scope: func_scope,
            functions: HashMap::new(),
        },
        vm: c.vm
    };
    for s in &func_decl.statements {
        try!(gen_statement(&mut func_context, s));
    }
}
