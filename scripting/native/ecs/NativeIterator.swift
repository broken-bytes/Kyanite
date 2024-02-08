@_implementationOnly import Bridge
import WinSDK

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
        OutputDebugStringA("Getting components from iterator\n")
        guard let buffer = ECS_GetComponentsFromIterator(handle, index + 1, MemoryLayout<T>.size) else {
            OutputDebugStringA("Failed to get components from iterator\n")
            fatalError("Failed to get components from iterator")
        }
        OutputDebugStringA("Got components from iterator\n")

        let ptr = UnsafeMutableBufferPointer<T>(
            start: buffer.assumingMemoryBound(to: T.self),
            count: size()
        )
        OutputDebugStringA("Returning components from iterator\n")
        return ptr
    }
}