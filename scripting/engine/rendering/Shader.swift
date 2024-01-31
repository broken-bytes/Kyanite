import Foundation

public class Shader: Object {
    internal var uuid: String

    required internal init(uuid: String) {
        self.uuid = uuid

        super.init(handle: AssetManager.shared.loadAsset(type: Shader.self, uuid: uuid))
    }

    public convenience init() {
        self.init(uuid: UUID().uuidString)
    }
}