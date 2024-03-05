import Foundation
import Native
import WinSDK

public struct Entity {
    public let id: UInt64
    public var name: String {
        NativeECS.shared.name(of: self.id)!
    }

    public var children: [Entity] {
        return []
    }

    public var parent: Entity? {
        guard let id = NativeECS.shared.parent(of: self.id) else {
            return nil
        }

        let name = NativeECS.shared.name(of: id)

        return Entity(id: id)
    }

    public var components: [any Component] {
        let componentIds = NativeECS.shared.getAllComponents(entity: self.id)
        OutputDebugStringA("Components: \(componentIds)\n")
        return []
    }

    // Used by the engine for creating non-world entities
    internal init(_ name: String) {
        self.id = NativeECS.shared.createEntity(name: name)
    }

    internal init(id: UInt64) {
        self.id = id
    }

    public init(name: String, parent: Entity? = nil) {
        self.id = NativeECS.shared.createEntity(name: name)
        
        if let parent {
            NativeECS.shared.setParent(child: self.id, parent: parent.id)
        } else {
            NativeECS.shared.setParent(child: self.id, parent: EngineEnvironment.shared.engineParent.id)
        }

        EventSystem.shared.emit(EntityLifetimeEvent(entity: self, isAlive: true))
    }

    public func addComponent<T: Component>(_ component: T.Type) {
        let componentId = _ComponentRegistry.shared._get(T.self)
        NativeECS.shared.addComponent(entity: self.id, componentId: componentId)
    }

    public func setComponent<T: Component>(_ component: inout T) {
        var componentId = _ComponentRegistry.shared._get(T.self)

        NativeECS.shared.setComponent(entity: self.id, componentId: componentId, component: &component)
    }

    public func getComponent<T: Component>(_ component: T.Type) -> UnsafeMutablePointer<T>? {
        let componentId = _ComponentRegistry.shared._get(T.self)
        return NativeECS.shared.getComponent(entity: self.id, componentId: componentId)
    }

    public func setParent(_ parent: Entity) {
        NativeECS.shared.setParent(child: self.id, parent: parent.id)
    }

    public static func find(by name: String) -> Entity? {
        guard let id = NativeECS.shared.find(by: name) else {
            return nil
        }

        return Entity(id: id)
    }
}

extension Entity: Sendable, Identifiable, Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(id)
    }

    public static func == (lhs: Entity, rhs: Entity) -> Bool {
        return lhs.id == rhs.id
    }
}