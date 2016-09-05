use ast::{Statement};
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
