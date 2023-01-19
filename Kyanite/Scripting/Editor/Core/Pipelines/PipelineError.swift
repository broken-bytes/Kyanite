public enum PipelineError: Error {
    case scriptError(message: String)
    case missingFile(message: String)
    case timeout
}