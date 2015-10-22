use super::tokenizer::Tokenizer;
use super::token::TokenType;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;
use std::rc::Rc;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{path: "if", token: TokenType::If},
    TokenDef{path: "else", token: TokenType::Else},
];

pub struct StringReader {
    string: String,
    keywords: FinalNode,
}

impl StringReader {
    pub fn new() -> StringReader {
        return StringReader{
            string: String::new(),
            keywords: symboltree::generate_tree(SYMBOLS)
        };
    }
}

impl Tokenizer for StringReader {
    fn reset(&mut self) {
    }

    fn read(&mut self, c: char, line_num: i32) -> bool {
        match c {
            'a'...'z' | 'A'...'Z' | '0'...'9' | '_' => {
                self.string.push(c);
                true
            },
            _ => false,
        }
    }

    fn publish(&mut self) -> Option<TokenType> {
        match self.keywords.find(&self.string) {
            Some(t) => Some(t),
            None => Some(TokenType::Symbol(Rc::new(self.string.clone()))),
        }
    }
}
