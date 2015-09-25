pub trait Statement {
    fn evaluate();
}

pub struct FunctionDeclaration {
    name: str,
}
