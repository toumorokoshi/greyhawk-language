use super::*;

#[test]
fn test_lexer() {
    let lexer = Lexer::new();
    match lexer.symbols.find("+") {
        Some(t) => {
            match t {
                token::Token::Plus => println!("great!"),
                _ => assert!(false),
            }
        },
        None => assert!(false),
    }
}
