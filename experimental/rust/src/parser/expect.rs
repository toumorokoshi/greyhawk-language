use lexer::token::TokenType;
use super::{Parser, PResult, ErrorMessage};

pub type ExpectResult<T> = Result<T, ErrorMessage>;

pub fn typ(parser: &mut Parser) -> PResult<String> {
    match try_token!(parser.next(), "type check".to_string()).typ {
        TokenType::Type(ref t) => Ok(t.clone()),
        ref t @ _ => vec![ErrorMessage{token: t.clone(), message: "expected a type.".to_string())]
    }
}

pub fn symbol(parser: &mut Parser) -> PResult<String> {
    match try_token!(parser.next(), "looking for symbol".to_string()).typ {
        TokenType::Symbol(ref t) => Ok(t.clone()),
        ref t @ _ => vec![ErrorMessage{token: Some(t), message: "expected a symbol".to_string())]
    }
}

pub fn expect<'a>(parser: &mut Parser, t: TokenType) -> PResult<()> {
    let next = try_token!(parser.next(), format!("expected a token {}", t));
    if next.typ == t {
        Ok(())
    } else {
        vec![ErrMessage(format!("expected a token of {}, found {}", t, next.typ))]
    }
}
