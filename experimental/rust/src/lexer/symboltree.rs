use super::token;
use std::collections::HashMap;
use std::str::Chars;
use std::rc::Rc;

pub struct TokenDef {
    pub path: &'static str,
    pub token: token::TokenType
}

pub struct Node {
    pub token: Option<token::TokenType>,
    pub children: HashMap<char, Node>,
}

pub struct FinalNode {
    pub token: Option<token::TokenType>,
    pub children: HashMap<char, Rc<FinalNode>>,
}

impl FinalNode {
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
            None => {
                self.token = Some(token)
            }
        }
    }

    pub fn finalize(&self) -> FinalNode {
        let mut children = HashMap::new();
        for (&key, value) in &self.children {
            children.insert(key, Rc::new(value.finalize()));
        }
        return FinalNode {
            token: self.token,
            children: children
        };
    }
}

// given a string of token defs, return a tree.
pub fn generate_tree(token_defs: &[TokenDef]) -> FinalNode {
    let mut root = Node::new();
    for token in token_defs {
        root.add_token(&mut token.path.chars(), token.token);
    }
    return root.finalize();
}
