public class _SystemRegistry {
    static let shared = _SystemRegistry()

    private init() {}

    internal var systems: [Int: UInt64] = [:]

    public func _register(name: String, runFunc: (@convention(c) (NativePointer?) -> Void)?) {
        Bridge_Engine_RegisterSystem(name.cString(using: .utf8), runFunc)
    }
}