import Core

internal typealias RegisterSystem = @convention(c) (UnsafePointer<Int8>, UnsafeMutableRawPointer, Bool, UnsafeMutableRawPointer, UInt64) -> UInt64
internal typealias GetSystemDelta = @convention(c) (UnsafeMutableRawPointer, UnsafeMutablePointer<Float>) -> Void
internal typealias GetSystemComponentData = @convention(c) (UnsafeMutableRawPointer, UInt64, UInt8, UnsafeMutablePointer<UInt64>) -> UnsafeMutableRawPointer
internal typealias GetSystemFromIter = @convention(c) (UnsafeMutableRawPointer) -> UInt64


protocol SystemDispatchable {
    associatedtype Dispatch
    var item: Dispatch { get }

    var block: UnsafeMutableRawPointer { get }
}

struct SingleComponentDispatch<C0: Component>: SystemDispatchable {
    typealias Dispatch = (C0.Type)

    var block: UnsafeMutableRawPointer

    let tuple: (C0.Type)
    var item: (C0.Type) { self.tuple }
}

struct TwoComponentDispatch<C0: Component, C1: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1)
    var item: (C0, C1) { self.tuple }
}

struct ThreeComponentDispatch<C0: Component, C1: Component, C2: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2)
    var item: (C0, C1, C2) { self.tuple }
}

struct FourComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2, C3)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2, C3)
    var item: (C0, C1, C2, C3) { self.tuple }
}

struct FiveComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2, C3, C4)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2, C3, C4)
    var item: (C0, C1, C2, C3, C4) { self.tuple }
}

struct SixComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2, C3, C4, C5)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2, C3, C4, C5)
    var item: (C0, C1, C2, C3, C4, C5) { self.tuple }
}

struct SevenComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2, C3, C4, C5, C6)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2, C3, C4, C5, C6)
    var item: (C0, C1, C2, C3, C4, C5, C6) { self.tuple }
}

struct EightComponentDispatch<C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component, C7: Component>: SystemDispatchable {
    typealias Dispatch = (C0, C1, C2, C3, C4, C5, C6, C7)
    var block: UnsafeMutableRawPointer

    let tuple: (C0, C1, C2, C3, C4, C5, C6, C7)
    var item: (C0, C1, C2, C3, C4, C5, C6, C7) { self.tuple }
}

fileprivate struct SystemDispatcher {
    static let lib: Library = Library.loadLibrary(at: "./Kyanite-Runtime.dll")
    static let register: RegisterSystem = lib.loadFunc(named: "ECS_RegisterSystem")
    static let getSystemDelta: GetSystemDelta = lib.loadFunc(named: "ECS_GetSystemDelta")
    static let getSystemComponentData: GetSystemComponentData = lib.loadFunc(named: "ECS_GetComponentData")
    static let getSystem: GetSystemFromIter = lib.loadFunc(named: "ECS_GetSystemFromIter")

    fileprivate static var systemMapping: [any SystemDispatchable] = []

    fileprivate func getSystemComponentSet<T: Component>(iterator: UnsafeMutableRawPointer, type: T.Type, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        var count: UInt64 = 0
        let rawData = SystemDispatcher.getSystemComponentData(iterator, UInt64(MemoryLayout<T>.size), index, &count)

        return UnsafeMutableBufferPointer(start: rawData.bindMemory(to: T.self, capacity: Int(count)), count: Int(count))
    }

    fileprivate static func dispatch(iterator: UnsafeMutableRawPointer) {
        let delta = UnsafeMutablePointer<Float>.allocate(capacity: 1)
        print("Getting delta")
        SystemDispatcher.getSystemDelta(iterator, delta)
        print("Got delta \(delta.pointee)")

        let systemId = SystemDispatcher.getSystem(iterator)
        print("Got system ID")
    }

    fileprivate static func registerSystem(name: String, multiThreaded: Bool, _ archetype: Component.Type...) -> UInt64 {
        return name.withCString { cStr in 
            let arch: [UInt64] = archetype.map { return try! ComponentRegistry.shared.resolveType(type: $0) }
            return arch.withUnsafeBufferPointer {
                return SystemDispatcher.register(
                    cStr, 
                    unsafeBitCast(dispatch, to: UnsafeMutableRawPointer.self), 
                    multiThreaded, 
                    UnsafeMutableRawPointer(mutating: $0.baseAddress)!, 
                    UInt64(arch.count)
                )
            }
        }
    }
}

public func System<C0>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>) -> Void) -> Void where C0: Component {
    let funcPtr = unsafeBitCast(block, to: UnsafeMutableRawPointer.self)
    SystemDispatcher.systemMapping.append(SingleComponentDispatch<C0>(block: funcPtr, tuple: (C0.self)))
}

public func System<C0, C1>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>) -> Void) -> Void where C0: Component, C1: Component {

}

public func System<C0, C1, C2>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>) -> Void) -> Void where C0: Component, C1: Component, C2: Component {

}

public func System<C0, C1, C2, C3>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>) -> Void) -> Void where C0: Component, C1: Component, C2: Component, C3: Component {

}

public func System<C0, C1, C2, C3, C4>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>, UnsafeMutablePointer<C4>) -> Void) -> Void where C0: Component, C1: Component, C2: Component, C3: Component, C4: Component {

}

public func System<C0, C1, C2, C3, C4, C5>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>, UnsafeMutablePointer<C4>, UnsafeMutablePointer<C5>) -> Void) -> Void where C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component {

}

public func System<C0, C1, C2, C3, C4, C5, C6>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>, UnsafeMutablePointer<C4>, UnsafeMutablePointer<C5>, UnsafeMutablePointer<C6>) -> Void) -> Void where C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component {

}

public func System<C0, C1, C2, C3, C4, C5, C6, C7>(_ name: String, _ block: (Float, UnsafeMutablePointer<C0>, UnsafeMutablePointer<C1>, UnsafeMutablePointer<C2>, UnsafeMutablePointer<C3>, UnsafeMutablePointer<C4>, UnsafeMutablePointer<C5>, UnsafeMutablePointer<C6>, UnsafeMutablePointer<C7>) -> Void) -> Void where C0: Component, C1: Component, C2: Component, C3: Component, C4: Component, C5: Component, C6: Component, C7: Component {

}
