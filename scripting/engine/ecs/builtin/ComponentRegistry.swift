import Foundation

public class _ComponentRegistry {
    static let shared = _ComponentRegistry()

    private init() {}

    internal var components: [Int: UInt64] = [:]

    public func _register<T: Component>(_ component: T.Type) {
        var id = Bridge_Engine_RegisterComponent("\(T.self)", MemoryLayout<T>.size, MemoryLayout<T>.alignment)
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()
        
        components[hash] = id
    }

    public func _get<T: Component>(_ component: T.Type) -> UInt64 {
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()

        guard let component = components[hash] else {
            fatalError("Component not registered")
        }
        
        return component
    }
}