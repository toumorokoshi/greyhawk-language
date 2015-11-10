mod expression;
mod statements;

use super::lexer;
use super::lexer::token::TokenType;
use super::codegen;
use std::slice::Iter;
use std::iter::Peekable;
use self::statements::StatResult;
use self::expression::ExprResult;

pub fn parse(tokens: &Vec<lexer::Token>) -> Vec<codegen::Statement> {
    let mut parser = Parser::new(tokens);
    let mut statements: Vec<codegen::Statement> = Vec::new();
    let stat = statements::parse_statement(&mut parser);
    match stat {
        Ok(s) => statements.push(s),
        Err(err) => { println!("unable to parse! {}", err); },
    }
    return statements;
}

fn expect_next(typ: TokenType, message: &'static str, tokens: &mut Peekable<Iter<lexer::Token>>) -> Result<(), &'static str> {
    match tokens.next() {
        Some(t) => {
            if typ == t.typ {
                Ok(())
            } else {
                Err(message)
            }
        },
        None => Err("unable to find next token")
    }
}


pub struct Parser<'a> {
    pub cur_token: lexer::Token,
    pub tokens: Peekable<Iter<'a, lexer::Token>>,
}

impl<'a> Parser<'a> {
    pub fn new(tokens: &Vec<lexer::Token>) -> Parser {
        let mut token_iter = tokens.iter();
        match token_iter.next() {
            Some(ref t) => Parser{cur_token: (*t).clone(), tokens: token_iter.peekable()},
            None => panic!("parser doesn't have multiple tokens.")
        }
    }

    pub fn next(&mut self) -> lexer::Token {
        let previous = self.cur_token.clone();
        self.cur_token = match self.tokens.next() {
            Some(ref t) => (*t).clone(),
            None => panic!("unable to find next token."),
        };
        previous
    }

    pub fn has_next(&mut self) -> bool {
        match self.tokens.peek() {
            Some(t) => true,
            None => false
        }
    }

    pub fn cur_typ(&self) -> TokenType {
        return self.cur_token.typ.clone();
    }

    pub fn expect_next(&self, typ: TokenType, message: &'static str) -> Result<(), &'static str> {
        if (self.cur_typ() == typ) {
            Err(message)
        } else {
            Ok(())
        }
    }
}
