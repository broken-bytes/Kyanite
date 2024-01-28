public struct Quaternion {
    public var x: Float
    public var y: Float
    public var z: Float
    public var w: Float

    public static var identity: Quaternion { get { Quaternion(x: 0, y: 0, z: 0, w: 1) } }

    public init(x: Float, y: Float, z: Float, w: Float) {
        self.x = x
        self.y = y
        self.z = z
        self.w = w
    }
}

extension Quaternion: Equatable, Hashable {
    public static func == (lhs: Quaternion, rhs: Quaternion) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}
