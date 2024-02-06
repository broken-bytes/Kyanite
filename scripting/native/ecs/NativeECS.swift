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

    public func addComponent(entity: UInt64, componentId: UInt64) {
        ECS_AddComponent(entity, componentId)
    }

    public func setComponent<T: Hashable>(entity: UInt64, componentId: UInt64, component: T) {
        let data = UnsafeMutablePointer<T>.allocate(capacity: 1)
        data.initialize(to: component)
        ECS_SetComponent(entity, componentId, data)
    }

    public func registerComponent<T: Hashable>(_ component: T.Type) -> UInt64 {
        return ECS_RegisterComponent("\(T.self)", MemoryLayout<T>.size, MemoryLayout<T>.alignment)    
    }

    public func registerSystem(name: String, components: inout [UInt64], runFunc: (@convention(c) (UnsafeMutableRawPointer?) -> Void)?) -> UInt64 {
        var count = components.count
        return components.withUnsafeMutableBufferPointer { ptr in 
            return ECS_RegisterSystem(name.cString(using: .utf8), ptr.baseAddress, count, runFunc)
        }
    }
}