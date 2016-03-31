use super::tokenizer::Tokenizer;
use super::token::TokenType;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;
use std::rc::Rc;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{path: "if", token: TokenType::If},
    TokenDef{path: "else", token: TokenType::Else},
    TokenDef{path: "return", token: TokenType::Return},

];

enum State {
    LookingForBeginning,
    LookingForEnd,
    Done
}

pub struct LiteralReader {
    string: String,
    state: State
}

impl LiteralReader {
    pub fn new() -> LiteralReader {
        return LiteralReader {
            string: String::new(),
            state: State::LookingForBeginning
        };
    }
}

impl Tokenizer for LiteralReader {
    fn reset(&mut self) {
        self.string = String::new();
        self.state = State::LookingForBeginning
    }

    fn read(&mut self, c: char, line_num: i32) -> bool {
        if let State::Done = self.state {
            return false;
        }

        match c {
            '"' => {
                let (result, next_state) = match self.state {
                    State::LookingForBeginning => (true, State::LookingForEnd),
                    State::LookingForEnd => (true, State::Done),
                    State::Done => (false, State::Done)
                };
                self.state = next_state;
                result
            },
            _ => match self.state {
                State::Done => false,
                _ => {self.string.push(c); true}
            }
        }
    }

    fn publish(&mut self) -> Vec<TokenType> {
        let token = TokenType::String(self.string.clone());
        self.reset();
        return vec![token];
    }
}
