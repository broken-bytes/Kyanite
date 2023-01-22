public class Resource {
    internal let uuid: String
    internal let refId: UInt64

    internal init(uuid: String, refId: UInt64) {
        self.uuid = uuid
        self.refId = refId
    }
}