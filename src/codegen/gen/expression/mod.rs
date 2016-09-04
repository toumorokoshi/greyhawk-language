mod array_create;
mod binop;
mod condition;
use super::Context;
use self::array_create::{
    gen_array, gen_index_set, gen_index_get
};
use ast::{Expression};
use self::binop::generate_binop;
use self::condition::gen_condition;
use vm::{LocalObject, Scope, Op};
use codegen::{CGError, CGResult};

pub fn gen_expression(c: &mut Context, e: &Expression) -> CGResult<LocalObject> {
    Ok(match e {
        &Expression::ArrayCreate(ref ac) => try!(gen_array(c, &ac)),
        &Expression::Condition(ref cond) => try!(gen_condition(c, &cond)),
        &Expression::ConstInt{value} => {
            let obj = c.block.scope.allocate_local(types::INT_TYPE.clone());
            c.block.ops.push(Op::IntLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstFloat{value} => {
            let obj = c.block.scope.allocate_local(types::FLOAT_TYPE.clone());
            c.block.ops.push(Op::FloatLoad{register: obj.index, constant: value});
            obj
        },
        &Expression::ConstString{ref value} => {
            let obj = c.block.scope.allocate_local(types::STRING_TYPE.clone());
            c.block.ops.push(Op::StringLoad{register: obj.index, constant: value.clone()});
            obj
        },
        &Expression::ItemGet(ref ig) => try!(gen_index_get(c, &ig)),
        &Expression::ItemSet(ref is) => try!(gen_index_set(c, &is)),
        &Expression::Symbol(ref value) => {
            match c.block.scope.get_local(&(value.clone())) {
                Some(obj) => obj,
                None => { return Err(CGError::new(format!("unable to find symbol {}", value)));}
            }
        },
        &Expression::BinOp(ref op) => try!(generate_binop(c, op)),
        &Expression::Call{ref name, args: ref arg_expressions} => {
            let func = c.block.scope.get_function(name);
            let mut args = Vec::new();
            for e in arg_expressions {
                args.push(try!(gen_expression(c, &e)));
            }
            let ret_val = c.block.scope.allocate_local(func.return_type());
            c.block.ops.push(Op::Call{func: func, args: args, target: ret_val.index});
            ret_val
        }
    })
}
