use super::token::*;
use std::str::Chars;
use std::iter::Peekable;

pub trait Tokenizer<'a> {
    fn reset(&'a mut self);
    fn read(&mut self, c: char, line_num: i32) -> bool;
    fn publish(&'a mut self) -> Option<Token>;
}

pub struct NumReader {
    value: i32,
    line_num: i32,
}

impl NumReader {
    pub fn new() -> NumReader {
        return NumReader{value: 0, line_num: -1};
    }
}

impl<'a> Tokenizer<'a> for NumReader {
    fn reset(&'a mut self) {
        self.value = 0;
    }

    /// reads the char provided, returns false
    /// if not a valid character.
    fn read(&mut self, c: char, line_num: i32) -> bool {
        if(c < '0' || c > '9') {
            return false;
        }
        self.value *= 10;
        self.value += (c as i32) - ('0' as i32);
        self.line_num = line_num;
        return true;
    }

    fn publish(&mut self) -> Option<Token> {
        let token = Token{
            typ: TokenType::Int(self.value),
            line_num: self.line_num
        };
        self.value = 0;
        return Some(token);
    }
}
