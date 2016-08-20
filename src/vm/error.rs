#[derive(Debug)]
pub struct VMError {
    /// A wrapper around the multiple errors
    /// that can occur within the VM.
    pub message: String
}

pub type VMResult<T> = Result<T, VMError>;

impl VMError {
    pub fn new(message: &str) -> VMError {
        VMError{message: String::from(message)}
    }
}
