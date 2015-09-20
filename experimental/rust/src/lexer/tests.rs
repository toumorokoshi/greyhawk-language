use super;

#[test]
fn test_lexer() {
    let lexer = super::Lexer::new();
    lexer.match("+");
}
