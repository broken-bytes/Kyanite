@_implementationOnly import Bridge
import WinSDK

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

    public func sizeOfIterator(iterator: UnsafeMutableRawPointer) -> Int {
        return ECS_GetIteratorSize(iterator)
    }

    public func get<T>(iterator: UnsafeMutableRawPointer, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        // Flecs uses 1-based indexing
        guard let buffer = ECS_GetComponentsFromIterator(iterator, index + 1, MemoryLayout<T>.size) else {
            OutputDebugStringA("Failed to get components from iterator\n")
            fatalError("Failed to get components from iterator")
        }

        return UnsafeMutableBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: sizeOfIterator(iterator: iterator)
        )
    }
}