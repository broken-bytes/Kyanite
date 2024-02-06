import Foundation
import Native

public struct Entity {
    private var id: UInt64

    private var components: [UInt64: any Hashable] = [:]

    public init(name: String) {
        self.id = NativeECS.shared.createEntity(name: name)
    }

    public func addComponent<T: Hashable>(_ component: T.Type) {
        let componentId = _ComponentRegistry.shared._get(T.self)
        NativeECS.shared.addComponent(entity: self.id, componentId: componentId)
    }

    public func setComponent<T: Hashable>(_ component: T) {
        let componentId = _ComponentRegistry.shared._get(T.self)

        NativeECS.shared.setComponent(entity: self.id, componentId: componentId, component: component)
    }
}