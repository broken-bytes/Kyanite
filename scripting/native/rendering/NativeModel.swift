public struct NativeModel {
    internal var uuid: String
    internal var meshIds: [UInt8]

    internal init(uuid: String, meshIds: [UInt8]) {
        self.uuid = uuid
        self.meshIds = meshIds
    }
}