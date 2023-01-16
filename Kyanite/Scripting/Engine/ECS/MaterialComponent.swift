public struct MaterialComponent {
    private let internalRefId: UInt64

    public init(with shaderId: UInt64) {
        // TODO: Create material on c++ core and get the ref 

        self.internalRefId = 0
    }
}