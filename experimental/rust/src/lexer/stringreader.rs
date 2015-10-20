use super::tokenizer::Tokenizer;
use super::token::TokenType;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{path: "if", token: TokenType::If},
    TokenDef{path: "else", token: TokenType::Else},
];

pub struct StringReader {
    chars: Vec<char>,
    keywords: FinalNode,
}

impl StringReader {
    pub fn new() -> StringReader {
        return StringReader{
            chars: Vec::new(),
            keywords: symboltree::generate_tree(SYMBOLS)
        };
    }
}

impl Tokenizer for StringReader {
    fn reset(&mut self) {
    }

    fn read(&mut self, c: char, line_num: i32) -> bool {
        match c {
            'a'...'z' | 'A'...'Z' => {
                self.chars.push(c);
                true
            },
            _ => false,
        }
    }

    fn publish(&mut self) -> Option<TokenType> {
        let string = self.chars.into_iter();
        match self.keywords.find(string) {
            Some(t) => None,
            None => None,
        }
    }
}
