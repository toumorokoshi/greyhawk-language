use super::tokenizer::Tokenizer;
use super::token::TokenType;
use std::rc::Rc;

pub struct TypeReader {
    name: String
}

impl TypeReader {
    pub fn new() -> TypeReader {
        return TypeReader {name: String::new()};
    }
}

impl Tokenizer for TypeReader {

    fn reset(&mut self) {}

    fn read(&mut self, c: char, line_num: i32) -> bool {
        match c {
            'a'...'z' | 'A'...'Z' | '0'...'9' | '_' => {
                self.name.push(c);
                true
            },
            _ => false,
        }
    }

    fn publish(&mut self) -> Option<TokenType> {
        Some(TokenType::Type(self.name.clone()))
    }
}
