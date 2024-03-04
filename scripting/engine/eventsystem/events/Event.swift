public protocol Event: Sendable {
    var type: Any.Type { get }
}