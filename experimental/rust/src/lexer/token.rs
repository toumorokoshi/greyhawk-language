use std::fmt;
use std::rc::Rc;

pub struct Token {
    pub typ: TokenType,
    pub line_num: i32
}

#[derive(Clone)]
pub enum TokenType {
    Int(i32),
    Float(f32),
    Plus,
    Sub,
    Mul,
    Div,
    Equal,
    Increment,
    If,
    Else,
    Symbol(Rc<String>),
    Type(Rc<String>),
    ParenL, ParenR,
}

fn fmt(typ: &TokenType, f: &mut fmt::Formatter) -> fmt::Result {
    match (typ) {
        &TokenType::Int(i) => write!(f, "Int: {}", i),
        &TokenType::Float(fl) => write!(f, "Float: {}", fl),
        &TokenType::Plus => write!(f, "Plus"),
        &TokenType::Sub => write!(f, "Minus"),
        &TokenType::Mul => write!(f, "Mul"),
        &TokenType::Div => write!(f, "Div"),
        &TokenType::Equal => write!(f, "Equal"),
        &TokenType::Increment => write!(f, "Increment"),
        &TokenType::If => write!(f, "If"),
        &TokenType::Else => write!(f, "Else"),
        &TokenType::Symbol(ref s) => write!(f, "Symbol: {}", s),
        &TokenType::Type(ref s) => write!(f, "Type: {}", s),
        &TokenType::ParenL => write!(f, "("),
        &TokenType::ParenR => write!(f, ")"),
    }
}

impl fmt::Debug for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        fmt(self, f)
    }
}

impl fmt::Display for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        fmt(self, f)
    }
}
