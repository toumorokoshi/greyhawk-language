use std::fmt;

pub struct Token {
    pub typ: TokenType,
    pub line_num: i32
}

#[derive(Copy, Clone)]
pub enum TokenType {
    Int(i32),
    Float(f32),
    Plus,
    Sub,
    Mul,
    Div,
    Equal,
    Increment,
}

impl fmt::Debug for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match (self) {
            &TokenType::Int(i) => write!(f, "Int: {}", i),
            &TokenType::Float(fl) => write!(f, "Float: {}", fl),
            &TokenType::Plus => write!(f, "Plus"),
            &TokenType::Sub => write!(f, "Minus"),
            &TokenType::Mul => write!(f, "Mul"),
            &TokenType::Div => write!(f, "Div"),
            &TokenType::Equal => write!(f, "Equal"),
            &TokenType::Increment => write!(f, "Increment"),
        }
    }
}

impl fmt::Display for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match (self) {
            &TokenType::Int(i) => write!(f, "Int: {}", i),
            &TokenType::Float(fl) => write!(f, "Float: {}", fl),
            &TokenType::Plus => write!(f, "Plus"),
            &TokenType::Sub => write!(f, "Minus"),
            &TokenType::Mul => write!(f, "Mul"),
            &TokenType::Div => write!(f, "Div"),
            &TokenType::Equal => write!(f, "Equal"),
            &TokenType::Increment => write!(f, "Increment"),
        }
    }
}
