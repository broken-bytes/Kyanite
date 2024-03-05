public class ReflectionInstace {
    private var reflection: Reflection
    private var data: UnsafeMutableRawPointer

    init(with reflection: Reflection, and data: UnsafeMutableRawPointer) {
        self.reflection = reflection
        self.data = data
    }

    internal func setValue<T>(for prop: String, value: inout T) {
        var dataPtr = data.advanced(by: reflection.offset(of: prop)).assumingMemoryBound(to: T.self)
        dataPtr.update(from: &value, count: 1)
    }

    internal func getValue<T>(of prop: String) -> T {
        data.advanced(by: reflection.offset(of: prop)).load(as: T.self)
    }
}