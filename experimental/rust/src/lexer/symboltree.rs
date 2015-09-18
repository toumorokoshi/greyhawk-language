use super::token;
use std::collections::HashMap;

pub struct TokenDef {
    pub path: &'static str,
    pub token: token::Token
}

pub struct Node {
    pub token: Option<token::Token>,
    pub children: HashMap<char, Node>,
}

impl Node {
    pub fn new() -> Node {
        Node{token: None, children: HashMap::new()}
    }

    pub fn add_token(&mut self, path: &str, token: token::Token) {
        let mut currentNode = self;
        for character in path.chars() {
            match currentNode.children.get(&character) {
                Some(child) => currentNode = child,
                None => {
                    let mut child = Node::new();
                    currentNode.children.insert(character, child);
                    currentNode = child;
                }
            }
        }
        currentNode.token = token;
    }
}

// given a string of token defs, return a tree.
pub fn generateTree(token_defs: &[TokenDef]) -> Node {
    let mut root = Node::new();
    for token in token_defs {
        root.add_token(token.path, token.token);
    }
}
