public class SystemIterator {

    internal var handle: NativePointer
    internal var size: Int = 0

    internal init(handle: NativePointer) {
        self.handle = handle
        self.size = ECS_GetIteratorSize(handle)
    }

    public func get<T>(index: UInt8) -> UnsafeMutableBufferPointer<T> {
        // Flecs uses 1-based indexing
        guard let buffer = ECS_GetComponentsFromIterator(handle, index + 1, MemoryLayout<T>.size) else {
            fatalError("Failed to get components from iterator")
        }

        return UnsafeMutableBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: size
        )
    }
}