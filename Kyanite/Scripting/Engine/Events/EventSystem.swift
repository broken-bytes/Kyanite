public class EventSystem<T> {
    
    private var queue: [T] = []
    private var subscriptions: [(T) -> Void] = []
    
    public init() {

    }

    public func subscribe(subscription: @escaping (T) -> Void) -> Int {
        subscriptions.append(subscription)
        return subscriptions.count - 1
    } 

    public func unsubscribe(id: Int) {
        subscriptions.remove(at: id)
    }

    public func flush() {
        queue = []
    }

    public func push(event: T) {
        queue.append(event)
        for sub in subscriptions {
            sub(event)
        }
    }
}