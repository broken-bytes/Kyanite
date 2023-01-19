public protocol Pipeline {
    func execute() throws -> Void
}