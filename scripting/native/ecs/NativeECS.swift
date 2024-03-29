@_implementationOnly import Bridge

public class NativeECS {
    public static let shared = NativeECS()

    private init() {}

    public func start(debug: Bool = false) {
        ECS_Init(nil, debug)
    }

    public func update(deltaTime: Float) {
        ECS_Update(deltaTime)
    }

    public func createEntity(name: String) -> UInt64 {
        return ECS_CreateEntity(name.cString(using: .utf8))
    }

    public func setParent(child: UInt64, parent: UInt64) {
        ECS_SetParent(child, parent)
    }

    public func parent(of entity: UInt64) -> UInt64? {
        let parentId = ECS_GetParent(entity)
        
        return parentId == 0 ? nil : parentId
    }

    public func addComponent(entity: UInt64, componentId: UInt64) {
        ECS_AddComponent(entity, componentId)
    }

    public func setComponent<T: Hashable>(entity: UInt64, componentId: UInt64, component: inout T) {
        ECS_SetComponent(entity, componentId, &component)
    }

    public func registerComponent<T: Hashable>(_ component: T.Type) -> UInt64 {
        return ECS_RegisterComponent("\(T.self)", MemoryLayout<T>.size, MemoryLayout<T>.alignment)    
    }

    public func registerSystem(name: String, components: inout [UInt64], runFunc: (@convention(c) (UnsafeMutableRawPointer?) -> Void)?) -> UInt64 {
        return ECS_RegisterSystem(name.cString(using: .utf8), &components, components.count, runFunc)
    }

    public func getComponent<T: Hashable>(entity: UInt64, componentId: UInt64) -> UnsafeMutablePointer<T>? {
        guard let component = ECS_GetComponent(entity, componentId) else {
            return nil
        }

        return component.assumingMemoryBound(to: T.self)
    }

    public func getComponentAsUnknown(entity: UInt64, componentId: UInt64) -> UnsafeMutableRawPointer? {
        guard let component = ECS_GetComponent(entity, componentId) else {
            return nil
        }

        return component
    }

    public func getAllComponents(
        entity: UInt64, 
        index: UInt64, 
        typeId: inout UInt64,
        data: UnsafeMutablePointer<UnsafeRawPointer?>
    ) {
        ECS_GetAllComponents(entity, index, &typeId, data)
    }

    public func sizeOfIterator(iterator: UnsafeMutableRawPointer) -> Int {
        return ECS_GetIteratorSize(iterator)
    }

    public func get<T>(iterator: UnsafeMutableRawPointer, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        // Flecs uses 1-based indexing
        guard let buffer = ECS_GetComponentsFromIterator(iterator, index + 1, MemoryLayout<T>.size) else {
            fatalError("Failed to get components from iterator")
        }

        return UnsafeMutableBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: sizeOfIterator(iterator: iterator)
        )
    }

    public func name(of entity: UInt64) -> String? {
        guard let name = ECS_GetEntityName(entity) else {
            return nil
        }
        
        return String(cString: name)
    }

    public func find(by name: String) -> UInt64? {
        let id = ECS_GetEntityByName(name)
        
        return id == 0 ? nil : id
    }
}