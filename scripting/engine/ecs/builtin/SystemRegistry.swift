public class _SystemRegistry {
    static let shared = _SystemRegistry()

    private init() {}

    internal var systems: [Int: UInt64] = [:]

    public func _register(name: String, components: [any Component.Type], runFunc: (@convention(c) (NativePointer?) -> Void)?) -> UInt64 {
        var componentIds: [UInt64] = []
        
        for component in components {
            componentIds.append(_ComponentRegistry.shared._get(component))
        }

        let count = componentIds.count
        let sysId = componentIds.withUnsafeMutableBufferPointer { ptr in 
            return Bridge_Engine_RegisterSystem(name.cString(using: .utf8), ptr.baseAddress, count, runFunc)
        }

        return sysId
    }
}