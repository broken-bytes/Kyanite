import Native

internal class GpuResourceManager {
    /// A struct that represents a GPU resource.
    struct GpuResource {
        /// The handle of the resource. This is the address of the resource in the GPU memory.
        let handle: UInt64
        /// The reference count of the resource. This is the number of references to the resource.
        let referenceCount: Int
    }

    internal static let shared = GpuResourceManager()
    private var resources: [String: GpuResource] = [:]

    private init() {}
}