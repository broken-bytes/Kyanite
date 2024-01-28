public struct Vec4<T> where T: Hashable, T: Comparable, T: Numeric {
    public var x: T
    public var y: T
    public var z: T
    public var w: T

    public static var zero: Vec4<T> { get { Vec4(x: 0, y: 0, z: 0, w: 0) } }

    public init(x: T, y: T, z: T, w: T) {
        self.x = x
        self.y = y
        self.z = z
        self.w = w
    }
}

typealias Vector4 = Vec4<Float>

extension Vec4: Hashable {
    public static func == (lhs: Vec4<T>, rhs: Vec4<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}