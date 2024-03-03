public class EventSystem {
    public static let shared = EventSystem()

    private init() {

    }

    public func subscribe(_ callback: @escaping (Event) -> Void) {
        // Subscribe to events
    }

    public func unsubscribe(_ callback: @escaping (Event) -> Void) {
        // Unsubscribe from events
    }

    public func emit(_ event: Event) {
        // Publish an event
    }
}