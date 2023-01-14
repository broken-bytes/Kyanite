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

    public func push(event: T) {
        for sub in subscriptions {
            sub(event)
        }
    }
}