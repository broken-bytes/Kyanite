public struct MeshComponent : Component {
    private var internalRefId: UInt64

    public init(mesh: Mesh) {
        internalRefId = mesh.refId
    }

    internal init(internalRefId: UInt64) {
        self.internalRefId = internalRefId
    }
}