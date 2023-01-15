public struct Mesh {
    private let internalRefId: UInt64

    private init(internalRefId: UInt64) {
        self.internalRefId = internalRefId
    }

    public static func makeMesh(with uuid: String) -> Mesh {
        // TODO: Load mesh from C++ world and get the refId
        let refId: UInt64 = 0

        return Mesh(internalRefId: refId)
    }
}