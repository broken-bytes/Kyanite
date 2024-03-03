public protocol Event: Sendable {
    var type: EventType { get }
}