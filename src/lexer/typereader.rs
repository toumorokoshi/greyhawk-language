use super::tokenizer::Tokenizer;
use super::token::TokenType;

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

    fn read(&mut self, c: char) -> bool {
        match c {
            'a'...'z' | 'A'...'Z' | '0'...'9' | '_' => {
                self.name.push(c);
                true
            },
            _ => false,
        }
    }

    fn publish(&mut self) -> Vec<TokenType> {
        vec![TokenType::Type(self.name.clone())]
    }
}
