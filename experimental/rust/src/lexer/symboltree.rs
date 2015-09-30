use super::token;
use std::collections::HashMap;
use std::str::Chars;

pub struct TokenDef {
    pub path: &'static str,
    pub token: token::TokenType
}

pub struct Node {
    pub token: Option<token::TokenType>,
    pub children: HashMap<char, Node>,
}

impl Node {
    pub fn new() -> Node {
        Node{token: None, children: HashMap::new()}
    }

    pub fn add_token(&mut self, path: &mut Chars, token: token::TokenType) {
        match path.next() {
            Some(c) => {
                let next_node = self.children
                    .entry(c).or_insert(Node::new());
                next_node.add_token(path, token);
            },
            None => self.token = Some(token)
        }
    }

    pub fn find(&self, input: &str) -> Option<token::TokenType> {
        let mut root = self;
        let mut chars = input.chars();
        loop {
            match chars.next() {
                Some(c) =>  {
                    match root.children.get(&c) {
                        Some(child) => root = child,
                        None => break,
                    }
                },
                None => break,
            }
        }
        return root.token;
    }
}

// given a string of token defs, return a tree.
pub fn generate_tree(token_defs: &[TokenDef]) -> Node {
    let mut root = Node::new();
    for token in token_defs {
        root.add_token(&mut token.path.chars(), token.token);
    }
    return root;
}
