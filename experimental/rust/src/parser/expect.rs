use lexer::token::TokenType;
use super::Parser;

pub type ExpectResult<T> = Result<T, &'static str>;

pub fn typ(parser: &mut Parser) -> ExpectResult<String> {
    match try_option!(parser.peek()).typ {
        TokenType::Type(ref t) => Ok(t.clone()),
        _ => Err("expected a type.")
    }
}

pub fn symbol(parser: &mut Parser) -> ExpectResult<String> {
    match try_option!(parser.peek()).typ {
        TokenType::Type(ref t) => Ok(t.clone()),
        _ => Err("expected a symbol.")
    }
}

pub fn expect(parser: &mut Parser, t: TokenType) -> ExpectResult<()> {
    if try_option!(parser.next()).typ == t {
        Ok(())
    } else {
        Err("expected a token of type.")
    }
}
