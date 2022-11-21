import Rendering

public struct MeshComponent: Component {
    // Mesh Instance
    public let meshes: [Mesh]
    public let materials: [Material]

    public init(meshes: [Mesh], materials: [Material]) {
        self.meshes = meshes
        self.materials = materials
    }
}
