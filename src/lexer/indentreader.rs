use super::tokenizer::Tokenizer;
use super::token::TokenType;

pub struct IndentReader {
    previous_indent: i8,
    current_indent: i8,
}

impl IndentReader {
    pub fn new() -> IndentReader {
        return IndentReader {
            previous_indent: 0,
            current_indent: 0,
        };
    }
}

impl Tokenizer for IndentReader {
    fn reset(&mut self) {
        self.previous_indent = self.current_indent;
        self.current_indent = 0;
    }

    fn read(&mut self, c: char) -> bool {
        match c {
            '\t' => {
                self.current_indent += 1;
                true
            }
            _ => false,
        }
    }

    fn publish(&mut self) -> Vec<TokenType> {
        let mut toks = Vec::new();
        let mut indent_diff = self.current_indent - self.previous_indent;
        while indent_diff > 0 {
            toks.push(TokenType::Indent);
            indent_diff -= 1;
        }
        while indent_diff < 0 {
            toks.push(TokenType::Unindent);
            indent_diff += 1;
        }
        self.reset();
        toks
    }
}
