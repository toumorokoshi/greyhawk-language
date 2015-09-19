pub mod token;
pub mod symboltree;
mod tests;

pub use self::token::*;
pub use self::symboltree::TokenDef;
pub use self::symboltree::generate_tree;
pub use self::symboltree::Node;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{ path: "+", token: token::Token::Plus},
    TokenDef{ path: "-", token: token::Token::Minus},
];

pub struct Lexer {
    symbols: Node
}

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer {
            symbols: generate_tree(SYMBOLS)
        };
    }
}
