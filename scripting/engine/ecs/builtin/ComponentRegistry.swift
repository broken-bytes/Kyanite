import Foundation
import Native

public class _ComponentRegistry {
    static let shared = _ComponentRegistry()

    private init() {}

    internal var components: [Int: UInt64] = [:]

    public func _register<T: Hashable>(_ component: T.Type) {
        let id = NativeECS.shared.registerComponent(T.self)
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()
        
        components[hash] = id
    }

    public func _get<T: Hashable>(_ component: T.Type) -> UInt64 {
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()

        guard let component = components[hash] else {
            fatalError("Component not registered")
        }
        
        return component
    }
}