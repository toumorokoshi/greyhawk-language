use lexer::token::TokenType;
use super::{Parser, PResult, ErrorMessage};

pub type ExpectResult<T> = Result<T, ErrorMessage>;

pub fn typ(parser: &mut Parser) -> PResult<String> {
    let next = try_token!(parser.next(), "type check".to_string());
    match next.typ {
        TokenType::Type(ref t) => Ok(t.clone()),
        _ => Err(vec![ErrorMessage{token: Some(next.clone()), message: "expected a type.".to_string()}])
    }
}

pub fn symbol(parser: &mut Parser) -> PResult<String> {
    let next = try_token!(parser.next(), "looking for symbol".to_string());
    match next.typ {
        TokenType::Symbol(ref t) => Ok(t.clone()),
        _ => Err(vec![ErrorMessage{token: Some(next.clone()), message: "expected a symbol".to_string()}])
    }
}

pub fn expect<'a>(parser: &mut Parser, t: TokenType) -> PResult<()> {
    let next = try_token!(parser.next(), format!("expected a token {}", t));
    if next.typ == t {
        Ok(())
    } else {
        Err(vec![ErrorMessage{message: format!("expected a token of {}, found {}", t, next.typ), token: Some(next)}])
    }
}
