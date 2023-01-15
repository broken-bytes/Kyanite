public struct Material {
    private let internalRefId: UInt64

    private init(internalRefId: UInt64) {
        self.internalRefId = internalRefId
    }

    public static func makeMaterial(with shaderId: UInt64) -> Material {
        // TODO: Create material from C++ world

        let refId: UInt64 = 0

        return Material(internalRefId: refId)
    }
}