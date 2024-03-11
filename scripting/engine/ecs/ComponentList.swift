import Foundation
@_implementationOnly import Native

public class ComponentList: IteratorProtocol, Sequence {
    public typealias Element = (type: any Component.Type, data: UnsafeRawPointer)

    private var index: UInt64 = 0
    private let entity: Entity

    internal init(entity: Entity) {
        self.entity = entity
    }

    public func next() -> (type: any Component.Type, data: UnsafeRawPointer)? {
        var typeId: UInt64 = 0
        var data: UnsafeRawPointer? = nil
        NativeECS.shared.getAllComponents(
            entity: entity.id,
            index: index,
            typeId: &typeId,
            data: &data
        )        

        guard let type = _ComponentRegistry.shared._get(typeId) else {
            return nil
        }

        if let data {
            index += 1
            return (type: type, data: data)
        }

        index = 0
        return nil
    }
}