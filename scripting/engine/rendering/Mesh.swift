import Foundation

public class Mesh: Object {
    internal var uuid: String

    internal init(uuid: String) {
        self.uuid = uuid

        super.init(handle: AssetManager.shared.loadAsset(uuid: uuid))
    }

    public init() {
        self.uuid = UUID().uuidString
    }
}