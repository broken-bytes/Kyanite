public class System {

    public init<C0>(_ name: String, _ block: @escaping (Float, UnsafeMutablePointer<C0>) -> Void) {
        let id = SystemDispatcher.registerSystem(name: name, multiThreaded: true, C0.self)
        withUnsafePointer(to: block) { 
            SystemDispatcher.systemMapping.append(SingleComponentDispatch<C0>(id: id, block: UnsafeMutableRawPointer(mutating: $0), tuple: (C0.self)))
        }
    }
}