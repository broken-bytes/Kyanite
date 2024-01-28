import Foundation

public struct Entity {
    private var id: UInt64

    public init(name: String) {
        self.id = Bridge_Engine_CreateEntity(name.cString(using: .utf8))
    }

    public func addComponent<T: Component>(_ component: T) {
        Bridge_Engine_AddComponent(self.id, _ComponentRegistry.shared._get(T.self))
    }
}