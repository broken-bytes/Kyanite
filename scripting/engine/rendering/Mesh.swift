import Foundation
import Native

public class Mesh: Asset<NativeMesh> {
    public var vertices: [Vertex] {
        get {
            var vertices: [Vertex] = []
            guard var ptr = native.vertices else {
                return []
            }  

            let result: [Vertex] = ptr.withMemoryRebound(to: Vertex.self) {
                vertices = Array($0)

                return vertices
            }

            return result
        } set {
            // TODO: Create a new buffer with the new vertices and update the vertex buffer.
        }
    }
    required override internal init(uuid: String, native: NativeMesh) {
        super.init(uuid: uuid, native: native)
    }
}

extension Mesh: Hashable {
    public static func == (lhs: Mesh, rhs: Mesh) -> Bool {
        return lhs.uuid == rhs.uuid
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(uuid)
    }
}