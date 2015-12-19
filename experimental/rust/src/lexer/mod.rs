pub mod token;
pub mod tokenizer;
pub mod indentreader;
pub mod symboltree;
pub mod symbolreader;
pub mod stringreader;
pub mod typereader;

#[cfg(test)]
mod tests;

pub use self::token::*;
pub use self::symboltree::TokenDef;
pub use self::symboltree::generate_tree;
pub use self::symboltree::Node;
pub use self::typereader::TypeReader;

use std::vec::Vec;

pub struct Lexer;

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer;
    }

    pub fn read(&self, input: &String) -> Vec<Token> {
        let mut tokens = Vec::new();
        let mut tokenizer: Option<Box<tokenizer::Tokenizer>> = None;
        let mut line_num = 1;

        // we create the tokenizers we need.
        let mut indentReader = Box::new(indentreader::IndentReader::new());
        let mut numReader = Box::new(tokenizer::NumReader::new());
        let mut stringReader = Box::new(stringreader::StringReader::new());
        let mut typeReader = Box::new(typereader::TypeReader::new());
        let mut symbolReader = Box::new(symbolreader::SymbolReader::new());

        for c in input.chars() {
            let mut to_set: Option<Box<tokenizer::Tokenizer>> = None;
            let mut clear = true;

            if c == '\n' {
                line_num += 1;
            }

            if let &mut Some(ref mut t) = &mut tokenizer {
                if !t.read(c, line_num) {
                    for tok in t.publish() {
                        tokens.push(Token{typ: tok, line_num: line_num});
                    }
                } else {
                    clear = false;
                }
            }

            if clear {
                let mut new_tokenizer: &Box<tokenizer::Tokenizer> = match c {
                    '0'...'9' => &numReader,
                    'a'...'z' => &stringReader,
                    'A'...'Z' => &typeReader,
                    '\t' => &indentReader,
                    _ => &symbolReader,
                };
                new_tokenizer.read(c, line_num);
                tokenizer = Some(new_tokenizer);
            }
        }

        if let &mut Some(ref mut t) = &mut tokenizer {
            for tok in t.publish() {
                tokens.push(Token{typ: tok, line_num: line_num});
            }
        }

        return tokens;
    }
}
