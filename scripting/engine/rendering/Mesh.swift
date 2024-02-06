import Foundation
import Native

public class Mesh: Asset<NativeMesh> {
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