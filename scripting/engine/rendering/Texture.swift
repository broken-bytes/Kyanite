import Foundation

public class Texture: Object {
    internal var uuid: String

    required internal init(uuid: String) {
        self.uuid = uuid

        super.init(handle: AssetManager.shared.loadAsset(uuid: uuid))
    }

    public convenience init() {
        self.init(uuid: UUID().uuidString)
    }
}