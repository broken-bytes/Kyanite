import Foundation

public class Mesh {
    internal var uuid: String

    internal init(uuid: String) {
        self.uuid = uuid
    }

    public init() {
        self.uuid = UUID().uuidString
    }
}