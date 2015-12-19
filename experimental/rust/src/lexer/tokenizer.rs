use super::token::*;
use std::str::Chars;
use std::iter::Peekable;

pub trait Tokenizer {
    fn reset(&mut self);
    fn read(&mut self, c: char, line_num: i32) -> bool;
    fn publish(&mut self) -> Vec<TokenType>;
}

pub enum NumReaderMode {
    ReadInt{value: i32},
    ReadDecimal{value: f32, power: f32},
}

pub struct NumReader {
    mode: NumReaderMode
}

impl NumReader {
    pub fn new() -> NumReader {
        return NumReader{mode: NumReaderMode::ReadInt{value: 0}};
    }
}

impl Tokenizer for NumReader {
    fn reset(&mut self) {
        self.mode = NumReaderMode::ReadInt{value: 0};
    }

    /// reads the char provided, returns false
    /// if not a valid character.
    fn read(&mut self, c: char, line_num: i32) -> bool {
        let mut mode = None;
        let result = match &mut self.mode {
            &mut NumReaderMode::ReadInt{ref mut value} => match c {
                '.' => {
                    mode = Some(NumReaderMode::ReadDecimal{
                        value: *value as f32, power: 1.0
                    });
                    true
                },
                '0' ... '9' => {
                    *value *= 10;
                    *value += (c as i32) - ('0' as i32);
                    true
                },
                _ => false,
            },
            &mut NumReaderMode::ReadDecimal{ref mut value, ref mut power} => match c {
                '0' ... '9' => {
                    *value *= 10 as f32;
                    *value += ((c as i32) - ('0' as i32)) as f32;
                    *power *= 10.0;
                    true
                },
                _ => false,
            }
        };
        match mode {
            Some(m) => self.mode = m,
            None => {},
        };
        result
    }

    fn publish(&mut self) -> Vec<TokenType> {
        let token = match self.mode {
            NumReaderMode::ReadInt{value} => TokenType::Int(value),
            NumReaderMode::ReadDecimal{value, power} => TokenType::Float(value / power),
        };
        self.reset();
        return vec![token];
    }
}
