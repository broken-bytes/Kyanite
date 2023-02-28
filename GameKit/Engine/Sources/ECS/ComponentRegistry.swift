internal class ComponentRegistry {
    
    internal static let shared = ComponentRegistry()
    
    private var indexCounter: UInt64 = 0
    private var registry: [String:UInt64] = [:]
    
    private init() {}

    internal func register<T>(component: T.Type) -> UInt64 {
        return registry.updateValue(indexCounter, forKey: "\(component.self)")!
    }

    internal func id<T>(for type: T.Type) -> UInt64 {
        return registry["\(type.self)"]!
    }
}