import Foundation
@_implementationOnly import Native

public class _ComponentRegistry {
    struct ComponentEntry {
        var id: UInt64
        var type: any Component.Type
    }
    public static let shared = _ComponentRegistry()

    private init() {}

    internal var components: [Int: ComponentEntry] = [:]

    public func _register<T: Component>(_ component: T.Type) {
        let id = NativeECS.shared.registerComponent(T.self)
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()
        
        components[hash] = ComponentEntry(id: id, type: T.self)
    }

    public func _get<T: Component>(_ component: T.Type) -> UInt64 {
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()

        guard let component = components[hash] else {
            fatalError("Component not registered")
        }
        
        return component.id
    }

    public func _get(_ id: UInt64) -> (any Component.Type)? {
        return components.first { $0.value.id == id }?.value.type
    }
}