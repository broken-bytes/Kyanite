import Core

internal typealias RegisterSystem = @convention(c) (UnsafePointer<Int8>, @convention(c) (UnsafeMutableRawPointer) -> Void, Bool, UnsafePointer<UInt64>, UInt64) -> UInt64
internal typealias GetSystemDelta = @convention(c) (UnsafeMutableRawPointer, UnsafeMutablePointer<Float>) -> Void
internal typealias GetSystemComponentData = @convention(c) (UnsafeMutableRawPointer, UInt64, UInt8, UnsafeMutablePointer<UInt64>) -> UnsafeMutableRawPointer
internal typealias GetSystemFromIter = @convention(c) (UnsafeMutableRawPointer) -> UInt64

func dispatch(iterator: UnsafeMutableRawPointer) {

    let systemId = SystemDispatcher.getSystem(iterator)

    guard let system = SystemDispatcher.systemMapping.first(where: { $0.id == systemId }) else { return }
    
    system.runnable(iterator)
}

internal struct SystemDispatcher {
    static let lib: Library = Core.Runtime.library
    static let register: RegisterSystem = lib.loadFunc(named: "ECS_RegisterSystem")
    static let getSystemDelta: GetSystemDelta = lib.loadFunc(named: "ECS_GetSystemDelta")
    static let getSystemComponentData: GetSystemComponentData = lib.loadFunc(named: "ECS_GetComponentData")
    static let getSystem: GetSystemFromIter = lib.loadFunc(named: "ECS_GetSystemFromIter")

    internal static var systemMapping: [System] = []

    internal static func getSystemComponentSet<T: Component>(iterator: UnsafeMutableRawPointer, type: T.Type, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        var count: UInt64 = 0
        let rawData = SystemDispatcher.getSystemComponentData(iterator, UInt64(MemoryLayout<T>.size), index, &count)

        return UnsafeMutableBufferPointer(start: rawData.bindMemory(to: T.self, capacity: Int(count)), count: Int(count))
    }

    internal static func getSystemComponentSetBySize<T: Component>(iterator: UnsafeMutableRawPointer, size: UInt64, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        var count: UInt64 = 0
        let rawData = SystemDispatcher.getSystemComponentData(iterator, size, index, &count)

        return UnsafeMutableBufferPointer(start: rawData.bindMemory(to: T.self, capacity: Int(count)), count: Int(count))
    }

    internal static func registerSystem(name: String, multiThreaded: Bool, _ archetype: Component.Type...) -> UInt64 {
        return name.withCString { cStr in 
            let arch: [UInt64] = archetype.map { return try! ComponentRegistry.shared.resolveType(type: $0) }
            return arch.withUnsafeBufferPointer {

                return SystemDispatcher.register(
                    cStr, 
                    dispatch, 
                    multiThreaded, 
                    $0.baseAddress!, 
                    UInt64(arch.count)
                )
            }
        }
    }
}
