use std::fmt;

#[derive(Clone, PartialEq)]
pub struct Token {
    pub typ: TokenType,
    pub line_num: i32
}

#[derive(Clone, PartialEq)]
pub enum TokenType {
    Int(i32),
    Float(f32),
    Plus,
    Sub,
    Mul,
    Div,
    Equal,
    Increment,
    Indent, Unindent,
    If,
    Else,
    String(String),
    Symbol(String),
    Type(String),
    ParenL, ParenR,
    Colon,
    Return,
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
            &TokenType::String(ref s) => format!("String: {}", s),
            &TokenType::Symbol(ref s) => format!("Symbol: {}", s),
            &TokenType::Type(ref s) => format!("Type: {}", s),
            &TokenType::ParenL => format!("("),
            &TokenType::ParenR => format!(")"),
            &TokenType::Indent => "Indent".to_string(),
            &TokenType::Unindent => "Unindent".to_string(),
            &TokenType::Colon => format!(":"),
            &TokenType::Return => format!("return"),
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
