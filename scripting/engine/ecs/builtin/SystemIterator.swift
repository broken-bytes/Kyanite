@_implementationOnly import Native

public class SystemIterator {
    internal var native: UnsafeMutableRawPointer
    public var size: Int { NativeECS.shared.sizeOfIterator(iterator: native) }

    public init(native: UnsafeMutableRawPointer) {
        self.native = native
    }

    public func get<T>(index: UInt8) -> UnsafeMutableBufferPointer<T> {
        return NativeECS.shared.get(iterator: native, index: index)
    }
}