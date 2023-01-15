// Core Funcs
internal typealias Start = @convention(c) (UInt32, UInt32, UnsafeMutableRawPointer) -> Void
internal typealias Update = @convention(c) (Float) -> Void
internal typealias SetRootDir = @convention(c) (UnsafeMutableRawPointer) -> Void

// Entity Funcs
internal typealias CreateEntity = @convention(c) (UnsafeMutableRawPointer) -> UInt64
internal typealias RegisterComponent = @convention(c) (UInt64, UInt8, UnsafeMutableRawPointer) -> UInt64
internal typealias AddComponent = @convention(c) (UInt64, UInt64, UInt64, UnsafeMutableRawPointer) -> UInt64
internal typealias GetComponent = @convention(c) (UInt64, UInt64) -> UnsafeMutableRawPointer


internal struct CoreFuncs {
    internal let start: Start
    internal let update: Update
    internal let setRootDir: SetRootDir
}

internal struct EntityFuncs {
    internal let createEntity: CreateEntity
    internal let registerComponent: RegisterComponent
    internal let addComponent: AddComponent
    internal let getComponent: GetComponent
}

public class NativeCore {
    public static let shared = NativeCore()

    private let lib: Library

    private let coreFuncs: CoreFuncs
    private let entityFuncs: EntityFuncs

    private init() {
        lib = Library.loadLibrary(at: "Kyanite-Runtime.dll")

        coreFuncs = CoreFuncs(
            start: self.lib.loadFunc(named: "Init"), 
            update: self.lib.loadFunc(named: "Update"),
            setRootDir: self.lib.loadFunc(named: "SetRootDir")
        )

        entityFuncs = EntityFuncs(
            createEntity: self.lib.loadFunc(named: "CreateEntity"), 
            registerComponent: self.lib.loadFunc(named: "RegisterComponent"), 
            addComponent: self.lib.loadFunc(named: "AddComponent"), 
            getComponent: self.lib.loadFunc(named: "GetComponent")
        )
    }

    public func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
        self.coreFuncs.start(width, height, window)
        self.coreFuncs.setRootDir(rootDir)
    }

    public func update(tick: Float) {
        self.coreFuncs.update(tick)
    }

    public func setRootDir(str: UnsafeMutableRawPointer) {
        self.coreFuncs.setRootDir(str)
    }

    public func createNewEntity(name: String) -> UInt64 {
        return withUnsafePointer(to: name.data(using: .utf8)) { ptr in 
            let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
            return self.entityFuncs.createEntity(rawPtr)
        }
    }

    public func registerNewComponent<T>(type: T.Type, named: String) -> UInt64 {
        return withUnsafePointer(to: named.data(using: .utf8)) { ptr in 
            let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
            return self.entityFuncs.registerComponent(UInt64(MemoryLayout<T>.size), UInt8(MemoryLayout<T>.alignment), rawPtr)
        }
    }

    public func addComponentToEntity<T>(entity: UInt64, component: UInt64, data: UnsafeMutableRawPointer, type: T.Type) -> UInt64 {
        return self.entityFuncs.addComponent(entity, component, UInt64(MemoryLayout<T>.size), data)
    }

    public func getComponentFromEntity(entity: UInt64, component: UInt64) -> UnsafeMutableRawPointer {
        return self.entityFuncs.getComponent(entity, component)
    }
}