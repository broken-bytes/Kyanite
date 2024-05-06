public struct Vec4<T>: Codable where T: Hashable, T: Comparable, T: Numeric, T: Codable {
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

    public init(_ x: T) {
        self.x = x
        self.y = x
        self.z = x
        self.w = x
    }

    public init(_ value: (T, T, T, T)) {
        self.x = value.0
        self.y = value.1
        self.z = value.2
        self.w = value.3
    }
}

public typealias Vector4 = Vec4<Float>

extension Vec4: Hashable {
    public static func == (lhs: Vec4<T>, rhs: Vec4<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w
    }
}