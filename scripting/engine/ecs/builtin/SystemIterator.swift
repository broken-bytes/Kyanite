import Native

public class SystemIterator {
    internal var native: NativeIterator
    public var size: Int { return native.size() }

    internal init(native: UnsafeMutableRawPointer) {
        self.native = native.load(as: NativeIterator.self)
    }

    public func get<T>(index: UInt8) -> UnsafeMutableBufferPointer<T> {
        return native.get(index: index)
    }
}