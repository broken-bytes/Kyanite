import Core

// Core Funcs
internal typealias Start = @convention(c) (UInt32, UInt32, UnsafeMutableRawPointer) -> Void
internal typealias Update = @convention(c) (Float) -> Void
internal typealias StartRender = @convention(c) () -> Void
internal typealias EndRender = @convention(c) () -> Void
internal typealias SetRootDir = @convention(c) (UnsafePointer<Int8>) -> Void
internal typealias SetResized = @convention(c) (UInt32, UInt32) -> Void

// Entity Funcs
internal typealias CreateEntity = @convention(c) (UnsafeMutableRawPointer) -> UInt64
internal typealias RegisterComponent = @convention(c) (UInt64, UInt8, UnsafeMutableRawPointer) -> UInt64
internal typealias AddComponent = @convention(c) (UInt64, UInt64, UInt64, UnsafeMutableRawPointer) -> UInt64
internal typealias GetComponent = @convention(c) (UInt64, UInt64) -> UnsafeMutableRawPointer
internal typealias RegisterSystem = @convention(c) (UnsafeMutableRawPointer, UnsafeMutableRawPointer, UnsafeMutableRawPointer, UInt64) -> Void
internal typealias GetComponentSetFromIterator = @convention(c) (UnsafeMutableRawPointer, UInt64, UInt8, UnsafeMutablePointer<UInt64>) -> UnsafeMutableRawPointer
internal typealias GetSystemDeltaTme = @convention(c) (UnsafeMutableRawPointer, UnsafeMutablePointer<Float>) -> Void
internal typealias SetMouseButtonDown = @convention(c) (UInt8) -> Void
internal typealias SetMouseButtonUp = @convention(c) (UInt8) -> Void
internal typealias SetMouseMoved = @convention(c) (Int32, Int32) -> Void
internal typealias StartIMGUIWindow = @convention(c) (UnsafePointer<Int8>) -> Void
internal typealias EndIMGUIWindow = @convention(c) () -> Void

internal struct CoreFuncs {
    internal let start: Start
    internal let update: Update
    internal let startRender: StartRender
    internal let endRender: EndRender
    internal let setRootDir: SetRootDir
    internal let setMouseUp: SetMouseButtonUp
    internal let setMouseDown: SetMouseButtonDown
    internal let setMouseMoved: SetMouseMoved
    internal let setResized: SetResized
}

internal struct EntityFuncs {
    internal let createEntity: CreateEntity
    internal let registerComponent: RegisterComponent
    internal let addComponent: AddComponent
    internal let getComponent: GetComponent
    internal let registerSystem: RegisterSystem
    internal let getComponentSetFromIterator: GetComponentSetFromIterator
    internal let getSystemDeltaTime: GetSystemDeltaTme
}

internal struct IMGUIFuncs {
    internal let startWindow: StartIMGUIWindow
    internal let endWindow: EndIMGUIWindow
}

enum EntityError: Error {
    case componentNotPoD(message: String)
    case invalidName(message: String)
}

internal class NativeCore {
    #if _EDITOR_RUNTIME
    internal static var shared: NativeCore! = nil
    #else
    internal static let shared = NativeCore()
    #endif

    private let lib: Library

    private let coreFuncs: CoreFuncs
    private let entityFuncs: EntityFuncs
    private let imguiFuncs: IMGUIFuncs

    private init() {
        lib = Library.loadLibrary(at: "Kyanite-Runtime.dll")

        coreFuncs = CoreFuncs(
            start: self.lib.loadFunc(named: "Init"), 
            update: self.lib.loadFunc(named: "Update"),
            startRender: self.lib.loadFunc(named: "StartRender"),
            endRender: self.lib.loadFunc(named: "EndRender"),
            setRootDir: self.lib.loadFunc(named: "SetRootDir"),
            setMouseUp: self.lib.loadFunc(named: "IMGUI_NotifyMouseUp"),
            setMouseDown: self.lib.loadFunc(named: "IMGUI_NotifyMouseDown"),
            setMouseMoved: self.lib.loadFunc(named: "IMGUI_NotifyMouseMove"),
            setResized: self.lib.loadFunc(named: "Resize")
        )

        entityFuncs = EntityFuncs(
            createEntity: self.lib.loadFunc(named: "CreateEntity"), 
            registerComponent: self.lib.loadFunc(named: "RegisterComponent"), 
            addComponent: self.lib.loadFunc(named: "AddComponent"), 
            getComponent: self.lib.loadFunc(named: "GetComponent"),
            registerSystem: self.lib.loadFunc(named: "RegisterSystem"),
            getComponentSetFromIterator: self.lib.loadFunc(named: "GetComponentData"),
            getSystemDeltaTime: self.lib.loadFunc(named: "GetSystemDelta")
        )

        imguiFuncs = IMGUIFuncs(
            startWindow: self.lib.loadFunc(named: "IMGUI_StartWindow"), 
            endWindow: self.lib.loadFunc(named: "IMGUI_EndWindow")
        )
    }

    internal func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, rootDir: UnsafePointer<Int8>) {
        self.coreFuncs.start(width, height, window)
        self.coreFuncs.setRootDir(rootDir)
    }

    internal func update(tick: Float) {
        self.coreFuncs.update(tick)
        self.coreFuncs.startRender()
    }

    internal func endUpdate() {
        self.coreFuncs.endRender()
    }

    internal func setRootDir(str: UnsafePointer<Int8>) {
        self.coreFuncs.setRootDir(str)
    }

    internal func createNewEntity(name: String) -> UInt64 {
        return withUnsafePointer(to: name.data(using: .utf8)) { ptr in 
            let rawPtr = UnsafeMutableRawPointer(mutating: ptr)
            return self.entityFuncs.createEntity(rawPtr)
        }
    }

    internal func registerNewComponent<T>(type: T.Type) throws -> UInt64 {
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

    internal func getComponentSet<T: Component>(iterator: UnsafeMutableRawPointer, type: T.Type, index: UInt8) -> UnsafeMutableBufferPointer<T> {
        var count: UInt64 = 0
        let data = self.entityFuncs.getComponentSetFromIterator(iterator, UInt64(MemoryLayout<T>.size), index, &count)

        let dataPtr = data.bindMemory(to: T.self, capacity: Int(count))
        let dataBuff = UnsafeMutableBufferPointer(start: dataPtr, count: Int(count))

        return dataBuff
    }

    internal func getSystemDeltaTime(iterator: UnsafeMutableRawPointer) -> Float {
        var delta: Float = 0.0
        self.entityFuncs.getSystemDeltaTime(iterator, &delta)
        return delta
    }

    internal func setMouseUp(button: UInt8) {
        self.coreFuncs.setMouseUp(button)
    }

    internal func setMouseDown(button: UInt8) {
        self.coreFuncs.setMouseDown(button)
    }

    internal func setMouseMoved(x: Int32, y: Int32) {
        self.coreFuncs.setMouseMoved(x, y)
    }

    internal func setResized(width: UInt32, height: UInt32) {
        self.coreFuncs.setResized(width, height)
    }
}