@_implementationOnly import Bridge

public struct NativeIterator {
    internal var handle: NativePointer

    internal init(handle: NativePointer) {
        self.handle = handle
    }

    public func size() -> Int {
        return ECS_GetIteratorSize(handle)
    }

    public func get<T>(index: UInt8) -> UnsafeMutableBufferPointer<T> {
        // Flecs uses 1-based indexing
        guard let buffer = ECS_GetComponentsFromIterator(handle, index + 1, MemoryLayout<T>.size) else {
            fatalError("Failed to get components from iterator")
        }

        return UnsafeMutableBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: size()
        )
    }
}