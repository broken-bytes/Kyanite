protocol AnySubscription { 
    var type: Event.Type { get }
}

public class EventSystem {
    class Subscription<T: Event>: AnySubscription {
        let callback: (T) -> Void
        let type: Event.Type

        init(callback: @escaping (T) -> Void, type: T.Type) {
            self.callback = callback
            self.type = type
        }
    }

    public static let shared = EventSystem()
    private var subscriptions: [AnySubscription] = []

    private init() {
    }

    public func subscribe<T: Event>(to type: T.Type, _ callback: @escaping (T) -> Void) {
        subscriptions.append(Subscription(callback: callback, type: type))
    }

    public func unsubscribe(_ callback: @escaping (Event) -> Void) {
        // Unsubscribe from events
    }

    public func emit<T: Event>(_ event: T) {
        for subscription in subscriptions {
            if subscription.type == type(of: event) {
                (subscription as! Subscription<T>).callback(event)
            }
        }
    }
}