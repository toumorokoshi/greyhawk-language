pub mod token;
pub mod tokenizer;
pub mod symboltree;
pub mod symbolreader;
pub mod stringreader;

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
                    if let Some(tok) = t.publish() {
                        tokens.push(Token{
                            typ: tok,
                            line_num: line_num
                        });
                    }
                } else {
                    clear = false;
                }
            }

            if clear {
                let mut new_tokenizer: Box<tokenizer::Tokenizer> = match c {
                    '0'...'9' => Box::new(tokenizer::NumReader::new()),
                    'a'...'z' | 'A'...'Z' => Box::new(stringreader::StringReader::new()),
                    _ => Box::new(symbolreader::SymbolReader::new()),
                };
                new_tokenizer.read(c, line_num);
                tokenizer = Some(new_tokenizer);
            }
        }

        if let &mut Some(ref mut t) = &mut tokenizer {
            if let Some(tok) = t.publish() {
                tokens.push(Token{ typ: tok, line_num: line_num});
            }
        }

        for token in &tokens {
            println!("{}", token.typ);
        }
        return tokens;
    }
}
