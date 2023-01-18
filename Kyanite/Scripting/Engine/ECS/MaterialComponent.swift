public struct MaterialComponent: Component {
    @EditorExposed private let internalRefId: UInt64

    public init(mesh: Material) {
        internalRefId = mesh.internalRefId
    }

    internal init(with shaderId: UInt64) {
        // TODO: Create material on c++ core and get the ref 

        self.internalRefId = 0
    }
}