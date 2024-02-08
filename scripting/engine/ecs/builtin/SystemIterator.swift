import Native
import WinSDK

public class SystemIterator {
    internal var native: UnsafeMutableRawPointer
    public var size: Int { NativeECS.shared.sizeOfIterator(iterator: native) }

    internal init(native: UnsafeMutableRawPointer) {
        self.native = native
    }

    public func get<T>(index: UInt8) -> UnsafeMutableBufferPointer<T> {
        return NativeECS.shared.get(iterator: native, index: index)
    }
}