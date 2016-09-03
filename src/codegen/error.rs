#[derive(Debug)]
pub struct CGError {
    /// A wrapper around the multiple errors
    /// that can occur within the VM.
    pub message: String
}

pub type CGResult<T> = Result<T, CGError>;

impl CGError {
    pub fn new(message: &str) -> CGError {
        CGError{message: String::from(message)}
    }
}
