import Foundation
@_implementationOnly import Native

public final class Mesh {
    public static var box: Mesh {
        return Mesh(uuid: "box")
    }

    public static var sphere: Mesh {
        return Mesh(uuid: "sphere")
    }

    public static var plane: Mesh {
        return Mesh(uuid: "plane")
    }

    internal let uuid: String

    public var vertices: [Vertex] {
        get {
            return []
        } set {
            // TODO: Create a new buffer with the new vertices and update the vertex buffer.
        }
    }

    required internal init(uuid: String) {
        self.uuid = uuid
    }
}

extension Mesh: Hashable, Equatable, Codable {
    public static func == (lhs: Mesh, rhs: Mesh) -> Bool {
        return lhs.uuid == rhs.uuid
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(uuid)
    }
}
