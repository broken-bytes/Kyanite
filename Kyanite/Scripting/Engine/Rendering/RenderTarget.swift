public struct RenderTarget {
    public let width: UInt32
    public let height: UInt32

    internal var resourceId: UInt64

    internal init(width: UInt32, height: UInt32) {
        self.width = width
        self.height = height
        self.resourceId = 0
        // TODO: Actually request RenderTexture from Engine API
    }
}