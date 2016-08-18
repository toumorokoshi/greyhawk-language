#[derive(Debug)]
pub struct VMError {
    /// A wrapper around the multiple errors
    /// that can occur within the VM.
    pub message: String
}

impl VMError {
}

pub type VMResult<T> = Result<T, VMError>;
