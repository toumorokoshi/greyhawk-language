use super::tokenizer::*;
use super::token::TokenType;
use super::token::Token;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;
use std::rc::Rc;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{ path: "+", token: TokenType::Plus},
    TokenDef{ path: "-", token: TokenType::Minus},
    TokenDef{ path: "==", token: TokenType::Equal},
    TokenDef{ path: "+=", token: TokenType::Increment},
];

pub struct SymbolReader {
    root: Rc<FinalNode>,
    current_node: Rc<FinalNode>,
    line_num: i32,
}

impl SymbolReader {
    pub fn new() -> SymbolReader {
        let root = Rc::new(symboltree::generate_tree(SYMBOLS));
        return SymbolReader{
            root: root.clone(),
            current_node: root.clone(),
            line_num: -1,
        };
    }
}

impl Tokenizer for SymbolReader {
    fn reset(& mut self) {
        // TODO: this is crazy inefficient. The tree is being
        // calculated every time.
        // we need to fix that.
        self.current_node = self.root.clone();
    }

    /// reads the char provided, returns false
    /// if not a valid character.
    fn read(&mut self, c: char, line_num: i32) -> bool {
        let mut next_node = self.current_node.clone();
        let was_read = match self.current_node.children.get(&c) {
            Some(n) => {
                next_node = n.clone();
                self.line_num = line_num;
                return true;
            },
            None => false,
        };
        self.current_node = next_node;
        return was_read;
    }

    fn publish(&mut self) -> Option<Token> {
        return match self.current_node.token {
            Some(token) => {
                return Some(Token {
                    typ: token,
                    line_num: self.line_num
                });
            },
            None => None,
        };
    }
}
