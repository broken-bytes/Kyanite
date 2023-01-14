import Foundation
import Core

public final class Entity {
    public var name: String
    public let uuid: String

    private let internalId: UInt64

    private var componentData: [UInt64: UnsafeMutableRawPointer] = [:]

    public init(name: String) {
        self.internalId = NativeCore.shared.createNewEntity(name: name)
        self.name = name
        self.uuid = UUID().uuidString
    }

    public func addComponent<T: Component>(component: T) {
        let typeId = ComponentRegistry.shared.resolve(component: T.self)
        let ptr = UnsafeMutablePointer<T>.allocate(capacity: 1)
        ptr.initialize(to: component)
        let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
        componentData[typeId] = rawPtr
        NativeCore.shared.addComponentToEntity(entity: self.internalId, component: typeId, data: rawPtr, type: T.self)
    }

    public func component<T>(of type: T.Type) -> UnsafeMutablePointer<T>? {
        let typeId = ComponentRegistry.shared.resolve(component: T.self)
        guard let rawPtr = componentData[typeId] else { return nil }
        
        return rawPtr.bindMemory(to: T.self, capacity: 1)
    }
}
