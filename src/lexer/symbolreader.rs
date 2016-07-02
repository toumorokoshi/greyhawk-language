use super::tokenizer::*;
use super::token::TokenType;
use super::token::Token;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;
use std::rc::Rc;

pub const SYMBOLS: &'static [TokenDef] = &[
    TokenDef{path: "+", token: TokenType::Plus},
    TokenDef{path: "-", token: TokenType::Sub},
    TokenDef{path: "*", token: TokenType::Mul},
    TokenDef{path: "/", token: TokenType::Div},
    TokenDef{path: "==", token: TokenType::Equal},
    TokenDef{path: "+=", token: TokenType::Increment},
    TokenDef{path: "(", token: TokenType::ParenL},
    TokenDef{path: ")", token: TokenType::ParenR},
    TokenDef{path: ":", token: TokenType::Colon},
    TokenDef{path: ":=", token: TokenType::Assign},
];

pub struct SymbolReader {
    root: Rc<FinalNode>,
    current_node: Rc<FinalNode>,
}

impl SymbolReader {
    pub fn new() -> SymbolReader {
        let root = Rc::new(symboltree::generate_tree(SYMBOLS));
        return SymbolReader{
            root: root.clone(),
            current_node: root.clone(),
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
        let mut was_read = false;
        match self.current_node.children.get(&c) {
            Some(n) => {
                next_node = n.clone();
                was_read = true;
            },
            None => {
                was_read = false;
            },
        };
        self.current_node = next_node;
        return was_read;
    }

    fn publish(&mut self) -> Vec<TokenType> {
        let tok = self.current_node.token.clone();
        self.reset();
        let mut v = Vec::new();
        if let Some(realTok) = tok {
            v.push(realTok);
        }
        return v;
    }
}
