public struct MaterialComponent: Component {
    private let internalRefId: UInt64

    public init(material: Material) {
        internalRefId = material.internalRefId
    }

    internal init(with shaderId: UInt64) {
        // TODO: Create material on c++ core and get the ref 

        self.internalRefId = 0
    }
}