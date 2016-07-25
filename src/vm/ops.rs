use super::function;
use super::scope::LocalObject;
use std::rc::Rc;
use std::fmt;

pub enum Op {
    ArrayCreate{target: usize, length: usize},
    ArraySet{source: usize, target: usize, index: u64},
    ArrayLoad{source: usize, target: usize, index: u64},
    Assign{target: usize, source: usize},
    BoolNot{source: usize, target: usize},
    // if the condition is true, continue down the registry.
    // if the condition is false, jump n instructions to the registry.
    Branch{condition: usize, if_false: usize},
    Call{func: Rc<function::Function>, args: Vec<LocalObject>, target: usize},
    FloatAdd{lhs: usize, rhs: usize, target: usize},
    FloatCmp{lhs: usize, rhs: usize, target: usize},
    FloatDiv{lhs: usize, rhs: usize, target: usize},
    FloatLoad{register: usize, constant: f64},
    FloatLessEqual{lhs: usize, rhs: usize, target: usize},
    FloatLessThan{lhs: usize, rhs: usize, target: usize},
    FloatMul{lhs: usize, rhs: usize, target: usize},
    FloatSub{lhs: usize, rhs: usize, target: usize},
    Goto{position: usize},
    IntAdd{lhs: usize, rhs: usize, target: usize},
    IntCmp{lhs: usize, rhs: usize, target: usize},
    IntDiv{lhs: usize, rhs: usize, target: usize},
    IntLessEqual{lhs: usize, rhs: usize, target: usize},
    IntLessThan{lhs: usize, rhs: usize, target: usize},
    IntLoad{register: usize, constant: i64},
    IntMul{lhs: usize, rhs: usize, target: usize},
    IntSub{lhs: usize, rhs: usize, target: usize},
    Noop{},
    StringLoad{register: usize, constant: Rc<String>},
    Return{register: usize},
}

impl Op {
    pub fn to_string(&self) -> String {
        match self {
            &Op::Assign{target, source} => format!("{0} <= {1}", target, source),
            &Op::ArrayCreate{target, length} => format!("{0} <= [{1}]", target, length),
            &Op::ArraySet{source, target, index} => format!("{1}[{2}] <= {0}", source, target, index),
            &Op::ArrayLoad{source, target, index} => format!("{0} <= {1}[{2}]", source, target, index),
            &Op::BoolNot{source, target} => format!("{1} = !{0}", source, target),
            &Op::Branch{condition, if_false} => format!("branch to {0} if {1} is false", condition, if_false),
            &Op::Call{ref func, args: _, target} => format!("{0} <= {1}()", target, func.name()),
            &Op::Goto{position} => format!("goto {0}", position),
            &Op::FloatAdd{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatCmp{lhs, rhs, target} => format!("{2} <= {0} == {1} (float)", lhs, rhs, target),
            &Op::FloatSub{lhs, rhs, target} => format!("{2} <= {0} - {1} (float)", lhs, rhs, target),
            &Op::FloatMul{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatDiv{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatLoad{register, constant} => format!("{0} <= {1} (float)", register, constant),
            &Op::FloatLessEqual{lhs, rhs, target} => format!("{2} <= {0} <= {1} (float)", lhs, rhs, target),
            &Op::FloatLessThan{lhs, rhs, target} => format!("{2} <= {0} < {1} (float)", lhs, rhs, target),
            &Op::IntAdd{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntCmp{lhs, rhs, target} => format!("{2} <= {0} == {1} (Int)", lhs, rhs, target),
            &Op::IntSub{lhs, rhs, target} => format!("{2} <= {0} - {1} (Int)", lhs, rhs, target),
            &Op::IntMul{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntDiv{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntLoad{register, constant} => format!("{0} <= {1} (Int)", register, constant),
            &Op::IntLessEqual{lhs, rhs, target} => format!("{2} <= {0} < {1} (int)", lhs, rhs, target),
            &Op::IntLessThan{lhs, rhs, target} => format!("{2} <= {0} <= {1} (int)", lhs, rhs, target),
            &Op::Noop{} => format!("noop"),
            &Op::StringLoad{register, ref constant} => format!("{0} <= {1} (String)", register, constant),
            &Op::Return{register} => format!("return {0}", register),
        }
    }
}

impl fmt::Display for Op {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&self.to_string())
    }
}
