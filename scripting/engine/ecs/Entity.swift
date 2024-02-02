import Foundation

public struct Entity {
    private var id: UInt64

    private var components: [UInt64: any Component] = [:]

    public init(name: String) {
        self.id = ECS_CreateEntity(name.cString(using: .utf8))
    }

    public func addComponent<T: Component>(_ component: T.Type) {
        let componentId = _ComponentRegistry.shared._get(T.self)
        ECS_AddComponent(self.id, componentId)

        // Convert the component data to a pointer
        let data = UnsafeMutablePointer<T>.allocate(capacity: 1)
        data.initialize(to: T())
        ECS_SetComponent(self.id, componentId, data)
    }

    public func setComponent<T: Component>(_ component: T) {
    }
}