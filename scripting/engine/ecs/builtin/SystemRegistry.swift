import Native

public class _SystemRegistry {
    public static let shared = _SystemRegistry()

    private init() {}

    internal var systems: [Int: UInt64] = [:]

    public func _register(name: String, components: [any Component.Type], runFunc: (@convention(c) (UnsafeMutableRawPointer?) -> Void)?) -> UInt64 {
        var componentIds: [UInt64] = []
        
        for component in components {
            componentIds.append(_ComponentRegistry.shared._get(component))
        }

        let sysId = NativeECS.shared.registerSystem(name: name, components: &componentIds, runFunc: runFunc)

        return sysId
    }
}