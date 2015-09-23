use std::fmt;

#[derive(Copy, Clone)]
pub enum Token {
    Int(i32),
    Plus,
    Minus,
    Equal,
    Increment,
}

impl fmt::Debug for Token {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match (self) {
            &Token::Int(i) => write!(f, "{}", i),
            &Token::Plus => write!(f, "Plus"),
            &Token::Minus => write!(f, "Minus"),
            &Token::Equal => write!(f, "Equal"),
            &Token::Increment => write!(f, "Increment"),
        }
    }
}
