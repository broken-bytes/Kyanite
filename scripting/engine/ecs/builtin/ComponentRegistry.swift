import Foundation
import Native
import WinSDK

public class _ComponentRegistry {
    struct ComponentEntry {
        var id: UInt64
        var type: any Hashable.Type
    }
    public static let shared = _ComponentRegistry()

    private init() {}

    internal var components: [Int: ComponentEntry] = [:]

    public func _register<T: Hashable>(_ component: T.Type) {
        let id = NativeECS.shared.registerComponent(T.self)
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()
        
        components[hash] = ComponentEntry(id: id, type: T.self)
    }

    public func _get<T: Hashable>(_ component: T.Type) -> UInt64 {
        var hasher = Hasher()
        hasher.combine(String(describing: T.self))
        let hash = hasher.finalize()

        guard let component = components[hash] else {
            fatalError("Component not registered")
        }
        
        return component.id
    }

    public func _get(id: UInt64) -> any Hashable.Type {
        OutputDebugStringA("Getting component type for id: \(id)\n")
        self.components.map { 
            OutputDebugStringA("Component: \($0.key) -> \($0.value)\n")
        }
        guard let component = components.first(where: { $0.key == id }) else {
            fatalError("Component not registered")
        }

        return component.value.type
    }
}