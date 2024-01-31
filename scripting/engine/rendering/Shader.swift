import Foundation

public class Shader: Object {
    internal var uuid: UUID

    required internal init(uuid: UUID) {
        self.uuid = uuid

        super.init(handle: AssetManager.shared.loadAsset(type: Shader.self, uuid: uuid.uuidString))
    }

    public convenience init() {
        self.init(uuid: UUID())
    }
}