pub mod token;
pub mod tokenizer;
pub mod indentreader;
pub mod symboltree;
pub mod symbolreader;
pub mod stringreader;
pub mod typereader;
mod literalreader;

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
    TypeReader,
    LiteralReader
}

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer;
    }

    pub fn read(&self, input: &String) -> Vec<Token> {
        let mut tokens = Vec::new();
        let mut line_num = 1;

        // we create the tokenizers we need.
        let mut indent_reader = indentreader::IndentReader::new();
        let mut num_reader = tokenizer::NumReader::new();
        let mut string_reader = stringreader::StringReader::new();
        let mut type_reader = typereader::TypeReader::new();
        let mut symbol_reader = symbolreader::SymbolReader::new();
        let mut literal_reader = literalreader::LiteralReader::new();
        let mut tokenizer_type: Option<TokenizerTypes> = None;

        let mut chars = input.chars();
        let mut next_char = chars.next();
        loop {
            match next_char {
                Some(c) => {
                    if let None = tokenizer_type {
                        tokenizer_type = match c {
                            '0'...'9' => Some(TokenizerTypes::NumReader),
                            'a'...'z' => Some(TokenizerTypes::StringReader),
                            'A'...'Z' => Some(TokenizerTypes::TypeReader),
                            '"' => Some(TokenizerTypes::LiteralReader),
                            '\t' | '\n' => Some(TokenizerTypes::IndentReader),
                            _ => Some(TokenizerTypes::SymbolReader),
                        };
                    }

                    let mut clear = false;
                    if let Some(ref t_type) = tokenizer_type {
                        let t: &mut tokenizer::Tokenizer = match t_type {
                            &TokenizerTypes::IndentReader => &mut indent_reader,
                            &TokenizerTypes::NumReader => &mut num_reader,
                            &TokenizerTypes::StringReader => &mut string_reader,
                            &TokenizerTypes::SymbolReader => &mut symbol_reader,
                            &TokenizerTypes::TypeReader => &mut type_reader,
                            &TokenizerTypes::LiteralReader => &mut literal_reader,
                        };

                        if !t.read(c) {
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
                    if clear { tokenizer_type = None; }
                },
                None => {
                    if let Some(ref t_type) = tokenizer_type {
                        let t: &mut tokenizer::Tokenizer = match t_type {
                            &TokenizerTypes::IndentReader => &mut indent_reader,
                            &TokenizerTypes::NumReader => &mut num_reader,
                            &TokenizerTypes::StringReader => &mut string_reader,
                            &TokenizerTypes::SymbolReader => &mut symbol_reader,
                            &TokenizerTypes::TypeReader => &mut type_reader,
                            &TokenizerTypes::LiteralReader => &mut literal_reader,
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
