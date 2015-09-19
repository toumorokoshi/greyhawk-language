use super::token;
use std::collections::HashMap;
use std::str::Chars;

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

    pub fn add_token(&mut self, path: &mut Chars, token: token::Token) {
        match path.next() {
            Some(c) => {
                let next_node = self.children
                    .entry(c).or_insert(Node::new());
                next_node.add_token(path, token);
            },
            None => self.token = Some(token)
        }
    }
}

// given a string of token defs, return a tree.
pub fn generateTree(token_defs: &[TokenDef]) -> Node {
    let mut root = Node::new();
    for token in token_defs {
        root.add_token(&mut token.path.chars(), token.token);
    }
    return root;
}
