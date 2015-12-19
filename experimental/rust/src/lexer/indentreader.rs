use super::tokenizer::Tokenizer;
use super::token::TokenType;
use super::symboltree::TokenDef;
use super::symboltree::FinalNode;
use super::symboltree;
use std::rc::Rc;

pub struct IndentReader {
    previousIndent: i8,
    currentIndent: i8,
}

impl IndentReader {
    pub fn new() -> IndentReader {
        return IndentReader {
            previousIndent: 0,
            currentIndent: 0,
        };
    }
}

impl Tokenizer for IndentReader {
    fn reset(&mut self) {
        self.previousIndent = self.currentIndent;
        self.currentIndent = 0;
    }

    fn read(&mut self, c: char, line_num: i32) -> bool {
        match c {
            '\t' => {
                self.currentIndent += 1;
                true
            }
            _ => false,
        }
    }

    fn publish(&mut self) -> Vec<TokenType> {
        let mut toks = Vec::new();
        let mut indentDiff = self.currentIndent - self.previousIndent;
        while (indentDiff > 0) {
            toks.push(TokenType::Indent);
        }
        while (indentDiff < 0) {
            toks.push(TokenType::Unindent);
        }
        self.reset();
        toks
    }
}
