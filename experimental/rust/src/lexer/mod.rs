pub mod token;
pub mod symboltree;

#[cfg(test)]
mod tests;

pub use self::token::*;
pub use self::symboltree::TokenDef;
pub use self::symboltree::generate_tree;
pub use self::symboltree::Node;

use std::vec::Vec;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{ path: "+", token: Token::Plus},
    TokenDef{ path: "-", token: Token::Minus},
    TokenDef{ path: "==", token: Token::Equal},
    TokenDef{ path: "+=", token: Token::Increment},
];

pub struct Lexer {
    pub symbols: Node,
}

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer {
            symbols: generate_tree(SYMBOLS)
        };
    }

    pub fn read(&self, input: &str) -> Vec<Token> {
        let mut tokens = Vec::new();
        tokens.push(token::Token::Int(10));
        tokens.push(token::Token::Plus);
        tokens.push(token::Token::Int(12));
        return tokens;
    }
}
