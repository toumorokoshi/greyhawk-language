pub mod token;
pub mod tokenizer;
pub mod symboltree;
pub mod symbolreader;

#[cfg(test)]
mod tests;

pub use self::token::*;
pub use self::symboltree::TokenDef;
pub use self::symboltree::generate_tree;
pub use self::symboltree::Node;

use std::vec::Vec;

pub struct Lexer;

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer;
    }

    pub fn read(&self, input: &String) -> Vec<Token> {
        let mut tokens = Vec::new();
        let mut tokenizer: Option<Box<tokenizer::Tokenizer>> = None;
        let mut line_num = 0;
        for c in input.chars() {
            let mut to_set: Option<Box<tokenizer::Tokenizer>> = None;
            let mut clear = true;

            if let &mut Some(ref mut t) = &mut tokenizer {
                if !t.read(c, line_num) {
                    match t.publish() {
                        Some(tok) => tokens.push(tok),
                        None => {},
                    };
                } else {
                    clear = false;
                }
            }

            if clear {
                let mut new_tokenizer: Box<tokenizer::Tokenizer> = match c {
                    c if ('0' <= c && c <= '9') => Box::new(tokenizer::NumReader::new()),
                    // this line breaks because it's set to tokenizer, which
                    // doesn't live long enough...
                    c => Box::new(symbolreader::SymbolReader::new()),
                };
                new_tokenizer.read(c, line_num);
                tokenizer = Some(new_tokenizer);
            }
        }
        for token in &tokens {
            println!("{}", token.typ);
        }
        return tokens;
    }

    /* pub fn read(&self, input: &String) -> Vec<Token> {
        let mut tokenizer: Option<Box<tokenizer::Tokenizer>> = None;
        let mut tokens = Vec::new();
        let mut line_num = 0;
        let mut chars = input.chars();
        for c in chars {
            let mut to_set: Option<Box<tokenizer::Tokenizer>> = None;
            let mut clear = false;
            match &mut tokenizer {
                &mut Some(ref mut t) => {
                    if !t.read(c, line_num) {
                        match t.publish() {
                            Some(tok) => tokens.push(tok),
                            None => {},
                        }
                        tokenizer = None;
                    }
                },
                &mut None => {
                    let mut t: Box<tokenizer::Tokenizer> = match c {
                        c if ('0' <= c && c <= '9') => Box::new(tokenizer::NumReader::new()),
                        c  => Box::new(symbolreader::SymbolReader::new()),
                    };
                    t.read(c, line_num);
                    tokenizer = Some(t);
                },
            }
            if (clear) {
                tokenizer = None;
            } else {
                match to_set {
                    Some(t) => tokenizer = Some(t),
                    None => {},
                }
            }
        }
        for token in &tokens {
            println!("{}", token.typ);
        }
        // tokens.push(Token{typ: token::TokenType::Int(10), line_num: 0});
        // tokens.push(Token{typ: token::TokenType::Plus, line_num: 0});
        // tokens.push(Token{typ: token::TokenType::Int(12), line_num: 0});
        return tokens;
    } */
}
