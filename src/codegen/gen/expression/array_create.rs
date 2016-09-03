use super::gen_expression;
use ast::{ArrayCreate, ItemGet, ItemSet};
use vm::{Op, scope, types, VM, LocalObject};
use codegen::{CGError, CGResult};

pub fn gen_array(c: &mut Context, array_create: &ArrayCreate) -> CGResult<LocalObject> {
    let size = try!(gen_expression(c, &array_create.size));
    if size.typ != types::INT_TYPE.clone() {
        return Err(
            CGError::new(format!("expected int for array argument, got {0}", size.typ))
        );
    }
    let typ = types::get_type_ref_from_string(&array_create.typ);
    let arr = scope.allocate_local(types::get_array_type(typ));
    ops.push(Op::ArrayCreate{target: arr.index, length_source: size.index});
    // add some tools needed for static initialization
    let one = scope.allocate_local(types::INT_TYPE.clone());
    ops.push(Op::IntLoad{register: one.index, constant: 1});
    let index = scope.allocate_local(types::INT_TYPE.clone());
    let mut i = 0;
    while i < array_create.values.len() {
        let ref e = array_create.values[i];
        let value = try!(gen_expression(c, e));
        ops.push(Op::ArraySet{
            source: value.index, target: arr.index, index_source: index.index
        });
        // increment our counter.
        ops.push(Op::IntAdd{lhs: index.index, rhs: one.index, target: index.index});
        i += 1;
    }
    Ok(arr)
}

pub fn gen_index_set(c: &mut Context, index_set: &ItemSet) -> CGResult<LocalObject> {
    let target = try!(gen_expression(vm, &index_set.target, scope, ops));
    let value = try!(gen_expression(vm, &index_set.value, scope, ops));
    let index = try!(gen_expression(vm, &index_set.index, scope, ops));
    // TODO: type mismatch.
    // TODO: index is not int check.
    ops.push(Op::ArraySet{source: value.index, target: target.index,
                          index_source: index.index});
    let result = scope.allocate_local(types::BOOL_TYPE.clone());
    ops.push(Op::IntLoad{register: result.index, constant: 1});
    Ok(result)
}

pub fn gen_index_get(c: &mut Context, index_get: &ItemGet) -> CGResult<LocalObject> {
    let source = try!(gen_expression(vm, &index_get.source, scope, ops));
    let index = try!(gen_expression(vm, &index_get.index, scope, ops));
    // TODO: index is not int check.
    let result = scope.allocate_local(source.typ.sub_types[0].clone());
    ops.push(Op::ArrayLoad{source: source.index, target: result.index,
                           index_source: index.index});
    Ok(result)
}
