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

enum TokenizerTypes {
    IndentReader,
    NumReader,
    StringReader,
    SymbolReader,
    TypeReader
}

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer;
    }

    pub fn read(&self, input: &String) -> Vec<Token> {
        let mut tokens = Vec::new();
        let mut line_num = 1;

        // we create the tokenizers we need.
        let mut indentReader = indentreader::IndentReader::new();
        let mut numReader = tokenizer::NumReader::new();
        let mut stringReader = stringreader::StringReader::new();
        let mut typeReader = typereader::TypeReader::new();
        let mut symbolReader = symbolreader::SymbolReader::new();
        let mut tokenizerType: Option<TokenizerTypes> = None;

        let mut chars = input.chars();
        let mut next_char = chars.next();
        loop {
            match next_char {
                Some(c) => {
                    if let None = tokenizerType {
                        tokenizerType = match c {
                            '0'...'9' => Some(TokenizerTypes::NumReader),
                            'a'...'z' => Some(TokenizerTypes::StringReader),
                            'A'...'Z' => Some(TokenizerTypes::TypeReader),
                            '\t' | '\n' => Some(TokenizerTypes::IndentReader),
                            _ => Some(TokenizerTypes::SymbolReader),
                        };
                    }

                    let mut clear = false;
                    if let Some(ref tType) = tokenizerType {
                        let t: &mut tokenizer::Tokenizer = match tType {
                            &TokenizerTypes::IndentReader => &mut indentReader,
                            &TokenizerTypes::NumReader => &mut numReader,
                            &TokenizerTypes::StringReader => &mut stringReader,
                            &TokenizerTypes::SymbolReader => &mut symbolReader,
                            &TokenizerTypes::TypeReader => &mut typeReader,
                        };

                        if !t.read(c, line_num) {
                            for tok in t.publish() {
                                tokens.push(Token{typ: tok, line_num: line_num});
                            }

                            if c == ' ' {
                                next_char = chars.next();
                            }
                            if c == '\n' {
                                line_num += 1;
                                next_char = chars.next();
                            }
                            clear = true;
                        } else {
                            next_char = chars.next();
                        }
                    }
                    if (clear) { tokenizerType = None; }
                },
                None => {
                    if let Some(ref tType) = tokenizerType {
                        let t: &mut tokenizer::Tokenizer = match tType {
                            &TokenizerTypes::IndentReader => &mut indentReader,
                            &TokenizerTypes::NumReader => &mut numReader,
                            &TokenizerTypes::StringReader => &mut stringReader,
                            &TokenizerTypes::SymbolReader => &mut symbolReader,
                            &TokenizerTypes::TypeReader => &mut typeReader,
                        };

                        for tok in t.publish() {
                            tokens.push(Token{typ: tok, line_num: line_num});
                        }
                    }
                    break;
                }
            }
        }
        return tokens;
    }
}
