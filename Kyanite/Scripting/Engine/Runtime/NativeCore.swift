import Core

// Core Funcs
internal typealias Start = @convention(c) (UInt32, UInt32, UnsafeMutableRawPointer) -> Void
internal typealias Update = @convention(c) (Float) -> Void
internal typealias SetRootDir = @convention(c) (UnsafeMutableRawPointer) -> Void

// Entity Funcs
internal typealias CreateEntity = @convention(c) (UnsafeMutableRawPointer) -> UInt64
internal typealias RegisterComponent = @convention(c) (UInt64, UInt8, UnsafeMutableRawPointer) -> UInt64
internal typealias AddComponent = @convention(c) (UInt64, UInt64, UInt64, UnsafeMutableRawPointer) -> UInt64
internal typealias GetComponent = @convention(c) (UInt64, UInt64) -> UnsafeMutableRawPointer
internal typealias RegisterSystem = @convention(c) (UnsafeMutableRawPointer, UnsafeMutableRawPointer, UnsafeMutableRawPointer, UInt64) -> Void
internal typealias GetComponentSetFromIterator = @convention(c) (UnsafeMutableRawPointer, UInt64, UInt8) -> UnsafeMutableRawPointer


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
    internal let registerSystem: RegisterSystem
    internal let getComponentSetFromIterator: GetComponentSetFromIterator
}

enum EntityError: Error {
    case componentNotPoD(message: String)
    case invalidName(message: String)
}

internal class NativeCore {
    internal static let shared = NativeCore()

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
            getComponent: self.lib.loadFunc(named: "GetComponent"),
            registerSystem: self.lib.loadFunc(named: "RegisterSystem"),
            getComponentSetFromIterator: self.lib.loadFunc(named: "GetComponentData")
        )
    }

    internal func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
        self.coreFuncs.start(width, height, window)
        self.coreFuncs.setRootDir(rootDir)
    }

    internal func update(tick: Float) {
        self.coreFuncs.update(tick)
    }

    internal func setRootDir(str: UnsafeMutableRawPointer) {
        self.coreFuncs.setRootDir(str)
    }

    internal func createNewEntity(name: String) -> UInt64 {
        return withUnsafePointer(to: name.data(using: .utf8)) { ptr in 
            let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
            return self.entityFuncs.createEntity(rawPtr)
        }
    }

    internal func registerNewComponent<T>(type: T.Type) throws -> UInt64 {
            var layout = ComponentLayout()

            let mirror = Mirror(reflecting: type)

            for child in mirror.children {
                // Go through every allowed type in order to check what type this prop is. Hacky but gets the job done for now
                if let _ = child.value as? Int8 {
                    layout.push(prop: .tiny)
                    continue
                }

                if let _ = child.value as? UInt8 {
                    layout.push(prop: .tiny)
                    continue
                }

                if let _ = child.value as? Character {
                    layout.push(prop: .small)
                    continue
                }

                if let _ = child.value as? Int16 {
                    layout.push(prop: .small)
                    continue
                }

                if let _ = child.value as? UInt16 {
                    layout.push(prop: .small)
                    continue
                }

                if let _ = child.value as? Int32 {
                    layout.push(prop: .regular)
                    continue
                }

                if let _ = child.value as? UInt32 {
                    layout.push(prop: .regular)
                    continue
                }

                if let _ = child.value as? Float {
                    layout.push(prop: .regular)
                    continue
                }

                if let _ = child.value as? Int64 {
                    layout.push(prop: .large)
                    continue
                }

                if let _ = child.value as? UInt64 {
                    layout.push(prop: .large)
                    continue
                }

                throw EntityError.componentNotPoD(message: "The component contains an invalid field(\(child.label ?? "UNKNOWN")). Please only use POD structs are components")
            }

        return "\(type)".withCString { 
            let rawPtr = UnsafeMutableRawPointer(mutating: $0)!
            return self.entityFuncs.registerComponent(UInt64(MemoryLayout<T>.size), UInt8(MemoryLayout<T>.alignment), rawPtr)   
        }
    }

    internal func addComponentToEntity<T>(entity: UInt64, component: UInt64, data: UnsafeMutableRawPointer, type: T.Type) -> UInt64 {
        return self.entityFuncs.addComponent(entity, component, UInt64(MemoryLayout<T>.size), data)
    }

    internal func getComponentFromEntity(entity: UInt64, component: UInt64) -> UnsafeMutableRawPointer {
        return self.entityFuncs.getComponent(entity, component)
    }

    internal func registerSystem(name: String, callback: @convention(c) (UnsafeMutableRawPointer) -> Void, _ archetype: Component.Type...) {
        let arch: [UInt64] = archetype.map { 
            return try! ComponentRegistry.shared.resolveType(type: $0)
        }

        let addRawPointer = unsafeBitCast(callback, to: UnsafeMutableRawPointer.self)
        guard let rawStr = name.data(using: .utf8) else { fatalError("Failed to create data from system name") }

        return name.withCString { cStr in       
            return arch.withUnsafeBufferPointer { 
                let rawPtr = UnsafeMutableRawPointer(mutating: $0.baseAddress)!
                let rawPtrStrName = UnsafeMutableRawPointer(mutating: cStr)!
                self.entityFuncs.registerSystem(rawPtrStrName, addRawPointer, rawPtr, UInt64(arch.count))
            }
        }
    }
}