use super::*;

#[test]
fn test_lexer() {
    let lexer = Lexer::new();
    match lexer.symbols.find("+") {
        Some(t) => {
            match t {
                token::TokenType::Plus => println!("great!"),
                _ => assert!(false),
            }
        },
        None => assert!(false),
    }
}

#[test]
fn test_lexer_double_equals() {
    let lexer = Lexer::new();
    match lexer.symbols.find("==") {
        Some(t) => {
            match t {
                token::TokenType::Equal => println!("great!"),
                _ => assert!(false),
            }
        },
        None => assert!(false),
    }
}

#[test]
fn test_lexer_double_increment() {
    let lexer = Lexer::new();
    match lexer.symbols.find("+=") {
        Some(t) => {
            match t {
                token::TokenType::Increment => println!("great!"),
                _ => assert!(false),
            }
        },
        None => assert!(false),
    }
}
