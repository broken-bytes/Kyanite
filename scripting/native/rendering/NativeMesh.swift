@_implementationOnly import Bridge

public class NativeMesh {
    internal let uuid: String

    internal var vertexBuffer: UInt64? = nil
    internal var indexBuffer: UInt64? = nil

    /// Load the vertices from the gpu buffer and return them as an array of vertices.
    /// The vertices are returned as a tuple of three arrays, one for each vertex attribute.
    /// Note: Only vertices that are loaded into the gpu buffer are returned. 
    /// If the mesh is not loaded into the gpu buffer, an empty array is returned.
    public var vertices: UnsafeMutableBufferPointer<Float>? {
        get {
            guard let vertexBuffer else {
                return nil
            }
            var ptr: NativePointer?
            var length: Int = 0
            Rendering_LoadVertexBuffer(vertexBuffer, &ptr, &length)

            var positions: [Float] = []
            var normals: [Float] = []
            var uvs: [Float] = []

            guard let ptr else {
                return nil
            }

            let floatPtr = ptr.bindMemory(to: Float.self, capacity: length)

            return UnsafeMutableBufferPointer(start: floatPtr, count: length)
        } set {
            guard let vertexBuffer else {
                return
            }

            guard let newValue else {
                return
            }

            Rendering_UpdateVertexBuffer(vertexBuffer, newValue.baseAddress, 8 * newValue.count)
        }
    }

    /// Creates a new instance pointing to a raw mesh data in memory.
    /// Note: The pointer should be freed after the usage.
    internal init(
        uuid: String, 
        vertices: UnsafeMutableBufferPointer<Float>, 
        indices: UnsafeMutableBufferPointer<UInt32>
    ) {
        self.uuid = uuid
        self.vertexBuffer = Rendering_CreateVertexBuffer(vertices.baseAddress, vertices.count)
        self.indexBuffer = Rendering_CreateIndexBuffer(indices.baseAddress, indices.count)

        // Deallocate the memory after the usage.
        vertices.deallocate()
        indices.deallocate()
    }
}