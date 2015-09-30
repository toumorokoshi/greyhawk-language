use std::fmt;

pub struct Token {
    pub typ: TokenType,
    pub line_num: i32
}

#[derive(Copy, Clone)]
pub enum TokenType {
    Int(i32),
    Plus,
    Minus,
    Equal,
    Increment,
}

impl fmt::Debug for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match (self) {
            &TokenType::Int(i) => write!(f, "{}", i),
            &TokenType::Plus => write!(f, "Plus"),
            &TokenType::Minus => write!(f, "Minus"),
            &TokenType::Equal => write!(f, "Equal"),
            &TokenType::Increment => write!(f, "Increment"),
        }
    }
}

impl fmt::Display for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match (self) {
            &TokenType::Int(i) => write!(f, "Int: {}", i),
            &TokenType::Plus => write!(f, "Plus"),
            &TokenType::Minus => write!(f, "Minus"),
            &TokenType::Equal => write!(f, "Equal"),
            &TokenType::Increment => write!(f, "Increment"),
        }
    }
}
