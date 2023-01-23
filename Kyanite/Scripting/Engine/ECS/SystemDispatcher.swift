import Core

internal typealias RegisterSystem = @convention(c) (UnsafePointer<Int8>, @convention(c) (UnsafeMutableRawPointer) -> Void, Bool, UnsafePointer<UInt64>, UInt64) -> UInt64
internal typealias GetSystemDelta = @convention(c) (UnsafeMutableRawPointer, UnsafeMutablePointer<Float>) -> Void
internal typealias GetSystemComponentData = @convention(c) (UnsafeMutableRawPointer, UInt64, UInt8, UnsafeMutablePointer<UInt64>) -> UnsafeMutableRawPointer
internal typealias GetSystemFromIter = @convention(c) (UnsafeMutableRawPointer) -> UInt64


protocol SystemDispatchable {
    associatedtype Dispatch
    var item: Dispatch { get }
    var id: UInt64 { get }

    var block: UnsafeMutableRawPointer { get }
}

struct SingleComponentDispatch<C0: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type)

    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct TwoComponentDispatch<C0: Component, C1: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct ThreeComponentDispatch<C0: Component, C1: Component, C2: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct FourComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type, C3.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct FiveComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type, C3.Type, C4.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct SixComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct SevenComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type, C6.Type)
    var block: UnsafeMutableRawPointer

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

struct EightComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component, C7: Component>: SystemDispatchable {
    var id: UInt64

    typealias Dispatch = (C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type, C6.Type, C7.Type)
    var block: (UnsafeMutableRawPointer)

    let tuple: Dispatch
    var item: Dispatch { self.tuple }
}

func dispatch(iterator: UnsafeMutableRawPointer) {
    let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
    SystemDispatcher.getSystemDelta(iterator, delta)

    let systemId = SystemDispatcher.getSystem(iterator)

    guard let system = SystemDispatcher.systemMapping.first(where: { $0.id == systemId }) else { return }

    let mirror = Mirror(reflecting: system.item)

    SystemBuilder.processEntity(components: () -> SystemContent)
}

internal struct SystemDispatcher {
    static let lib: Library = Library.loadLibrary(at: "./Kyanite-Runtime.dll")
    static let register: RegisterSystem = lib.loadFunc(named: "ECS_RegisterSystem")
    static let getSystemDelta: GetSystemDelta = lib.loadFunc(named: "ECS_GetSystemDelta")
    static let getSystemComponentData: GetSystemComponentData = lib.loadFunc(named: "ECS_GetComponentData")
    static let getSystem: GetSystemFromIter = lib.loadFunc(named: "ECS_GetSystemFromIter")

    internal static var systemMapping: [any SystemDispatchable] = []

    internal func getSystemComponentSet<T: Component>(iterator: UnsafeMutableRawPointer, type: T.Type, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        var count: UInt64 = 0
        let rawData = SystemDispatcher.getSystemComponentData(iterator, UInt64(MemoryLayout<T>.size), index, &count)

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
