use super::function;
use super::scope::LocalObject;
use std::rc::Rc;
use std::fmt;

pub enum Op {
    Assign{target: usize, source: usize},
    Call{func: Rc<function::Function>, args: Vec<LocalObject>, target: usize},
    FloatAdd{lhs: usize, rhs: usize, target: usize},
    FloatSub{lhs: usize, rhs: usize, target: usize},
    FloatMul{lhs: usize, rhs: usize, target: usize},
    FloatDiv{lhs: usize, rhs: usize, target: usize},
    FloatLoad{register: usize, constant: f64},
    IntAdd{lhs: usize, rhs: usize, target: usize},
    IntSub{lhs: usize, rhs: usize, target: usize},
    IntMul{lhs: usize, rhs: usize, target: usize},
    IntDiv{lhs: usize, rhs: usize, target: usize},
    IntLoad{register: usize, constant: i64},
    StringLoad{register: usize, constant: Rc<String>},
    Return{register: usize},
}

impl Op {
    pub fn to_string(&self) -> String {
        match self {
            &Op::Assign{target, source} => format!("{0} <= {1}", target, source),
            &Op::Call{ref func, ref args, target} => format!("{0} <= Call()", target),
            &Op::FloatAdd{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatSub{lhs, rhs, target} => format!("{2} <= {0} - {1} (float)", lhs, rhs, target),
            &Op::FloatMul{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatDiv{lhs, rhs, target} => format!("{2} <= {0} + {1} (float)", lhs, rhs, target),
            &Op::FloatLoad{register, constant} => format!("{0} <= {1} (float)", register, constant),
            &Op::IntAdd{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntSub{lhs, rhs, target} => format!("{2} <= {0} - {1} (Int)", lhs, rhs, target),
            &Op::IntMul{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntDiv{lhs, rhs, target} => format!("{2} <= {0} + {1} (Int)", lhs, rhs, target),
            &Op::IntLoad{register, constant} => format!("{0} <= {1} (Int)", register, constant),
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
