import Foundation

public class Mesh: Object {
    internal var uuid: UUID

    required internal init(uuid: UUID) {
        self.uuid = uuid

        super.init(handle: AssetManager.shared.loadAsset(type: Mesh.self, uuid: uuid.uuidString))
    }

    public convenience init() {
        self.init(uuid: UUID())
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