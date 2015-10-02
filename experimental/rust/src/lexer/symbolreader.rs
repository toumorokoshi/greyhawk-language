use super::tokenizer::*;
use super::token::TokenType;
use super::token::Token;
use super::symboltree::TokenDef;
use super::symboltree::Node;
use super::symboltree;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{ path: "+", token: TokenType::Plus},
    TokenDef{ path: "-", token: TokenType::Minus},
    TokenDef{ path: "==", token: TokenType::Equal},
    TokenDef{ path: "+=", token: TokenType::Increment},
];

pub struct SymbolReader<'a> {
    root: Node,
    current_node: &'a Node,
    line_num: i32,
}

impl<'a> SymbolReader<'a> {
    pub fn new() -> SymbolReader<'a> {
        let root = symboltree::generate_tree(SYMBOLS);
        return SymbolReader{
            root: root,
            current_node: &root,
            line_num: -1,
        };
    }
}

impl<'a> Tokenizer<'a> for SymbolReader<'a> {
    fn reset(&'a mut self) {
        // TODO: this is crazy inefficient. The tree is being
        // calculated every time.
        // we need to fix that.
        self.current_node = &self.root;
    }

    /// reads the char provided, returns false
    /// if not a valid character.
    fn read(&mut self, c: char, line_num: i32) -> bool {
        let val = match self.current_node.children.get(&c) {
            Some(n) => {
                self.current_node = n;
                self.line_num = line_num;
                return true;
            },
            None => false,
        };
        return val;
    }

    fn publish(&'a mut self) -> Option<Token> {
        return match self.current_node.token {
            Some(token) => {
                let return_val = Some(Token {
                    typ: token,
                    line_num: self.line_num
                });
                self.current_node = &self.root;
                return return_val;
            },
            None => None,
        }
    }
}
