pub mod token;
pub mod symboltree;

#[cfg(test)]
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
    pub symbols: Node,
}

impl Lexer {

    pub fn new() -> Lexer {
        return Lexer {
            symbols: generate_tree(SYMBOLS)
        };
    }

    pub fn read(&self, input: &str) {
        let mut root = &self.symbols;
        let mut chars = input.chars();
        loop {
            match chars.next() {
                Some(c) => {
                    match root.children.get(&c) {
                        Some(child) => root = child,
                        None => break,
                    }
                },
                None => break,
            }
        }

        match root.token {
            Some(token) => println!("found a token!"),
            None => println!("no token found.")
        }
    }
}
