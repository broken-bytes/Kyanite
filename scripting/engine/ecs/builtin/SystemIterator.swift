public class SystemIterator {

    internal var handle: NativePointer
    internal var size: Int = 0

    internal init(handle: NativePointer) {
        self.handle = handle
        self.size = Bridge_Engine_GetIteratorSize(handle)
    }

    public func get<T>(index: UInt8) -> UnsafeBufferPointer<T> {
        // Flecs uses 1-based indexing
        guard let buffer = Bridge_Engine_GetComponentsFromIterator(handle, index + 1, MemoryLayout<T>.size) else {
            fatalError("Failed to get components from iterator")
        }

        return UnsafeBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: size
        )
    }
}