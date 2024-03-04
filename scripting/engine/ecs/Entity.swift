import Foundation
import Native

public struct Entity: Sendable {
    public let id: UInt64
    public let name: String

    private var components: [UInt64: any Hashable] = [:]

    // Used by the engine for creating non-world entities
    internal init(_ name: String) {
        self.id = NativeECS.shared.createEntity(name: name)
        self.name = name
    }

    public init(name: String, parent: Entity? = nil) {
        self.id = NativeECS.shared.createEntity(name: name)
        self.name = name
        EventSystem.shared.emit(EntityLifetimeEvent(entity: self, isAlive: true))
        
        if let parent {
            NativeECS.shared.setParent(child: self.id, parent: parent.id)
        } else {
            NativeECS.shared.setParent(child: self.id, parent: EngineEnvironment.shared.engineParent.id)
        }
    }

    public func addComponent<T: Hashable>(_ component: T.Type) {
        let componentId = _ComponentRegistry.shared._get(T.self)
        NativeECS.shared.addComponent(entity: self.id, componentId: componentId)
    }

    public func setComponent<T: Hashable>(_ component: inout T) {
        var componentId = _ComponentRegistry.shared._get(T.self)

        NativeECS.shared.setComponent(entity: self.id, componentId: componentId, component: &component)
    }

    public func getComponent<T: Hashable>(_ component: T.Type) -> UnsafeMutablePointer<T>? {
        let componentId = _ComponentRegistry.shared._get(T.self)
        return NativeECS.shared.getComponent(entity: self.id, componentId: componentId)
    }

    public func setParent(_ parent: Entity) {
        NativeECS.shared.setParent(child: self.id, parent: parent.id)
    }
}