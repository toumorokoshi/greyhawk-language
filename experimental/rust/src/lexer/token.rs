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
    Colon,
}

impl TokenType {
    pub fn to_string(&self) -> String {
        match (self) {
            &TokenType::Int(i) => format!("Int: {}", i),
            &TokenType::Float(fl) => format!("Float: {}", fl),
            &TokenType::Plus => format!("Plus"),
            &TokenType::Sub => format!("Minus"),
            &TokenType::Mul => format!("Mul"),
            &TokenType::Div => format!("Div"),
            &TokenType::Equal => format!("Equal"),
            &TokenType::Increment => format!("Increment"),
            &TokenType::If => format!("If"),
            &TokenType::Else => format!("Else"),
            &TokenType::Symbol(ref s) => format!("Symbol: {}", s),
            &TokenType::Type(ref s) => format!("Type: {}", s),
            &TokenType::ParenL => format!("("),
            &TokenType::ParenR => format!(")"),
            &TokenType::Colon => format!(":"),
        }
    }
}

impl fmt::Debug for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&self.to_string())
    }
}

impl fmt::Display for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        f.write_str(&self.to_string())
    }
}
