use super::super::evaluate_expr;
use ast::{ArrayCreate, ItemGet, ItemSet};
use vm::{Op, scope, types};

pub fn gen_array(array_create: &ArrayCreate, scope: &mut scope::Scope,
                    ops: &mut Vec<Op>) -> scope::LocalObject {
    let size = evaluate_expr(&array_create.size, scope, ops);
    if size.typ != types::INT_TYPE.clone() {
        panic!(format!("expected int for array argument, got {0}", size.typ));
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
        let value = evaluate_expr(e, scope, ops);
        ops.push(Op::ArraySet{
            source: value.index, target: arr.index, index_source: index.index
        });
        // increment our counter.
        ops.push(Op::IntAdd{lhs: index.index, rhs: one.index, target: index.index});
        i += 1;
    }
    arr
}

pub fn gen_index_set(index_set: &ItemSet, scope: &mut scope::Scope,
                     ops: &mut Vec<Op>) -> scope::LocalObject {
    let target = evaluate_expr(&index_set.target, scope, ops);
    let value = evaluate_expr(&index_set.value, scope, ops);
    let index = evaluate_expr(&index_set.index, scope, ops);
    // TODO: type mismatch.
    // TODO: index is not int.
    ops.push(Op::ArraySet{source: value.index, target: target.index,
                          index_source: index.index});
    let result = scope.allocate_local(types::BOOL_TYPE.clone());
    ops.push(Op::IntLoad{register: result.index, constant: 1});
    result
}

pub fn gen_index_get(index_get: &ItemGet, scope: &mut scope::Scope,
                     ops: &mut Vec<Op>) -> scope::LocalObject {
    let source = evaluate_expr(&index_get.source, scope, ops);
    let index = evaluate_expr(&index_get.index, scope, ops);
    // TODO: index is not int.
    let result = scope.allocate_local(source.typ.sub_types[0].clone());
    ops.push(Op::ArrayLoad{source: source.index, target: result.index,
                           index_source: index.index});
    result
}
