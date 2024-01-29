public struct Vec3<T> where T: Hashable, T: Comparable, T: Numeric {
    public var x: T
    public var y: T
    public var z: T

    public static var zero: Vec3<T> { get { Vec3(x: 0, y: 0, z: 0) } }

    public init(x: T, y: T, z: T) {
        self.x = x
        self.y = y
        self.z = z
    }
}

public typealias Vector3 = Vec3<Float>

extension Vec3: Hashable {
    public static func == (lhs: Vec3<T>, rhs: Vec3<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
    }
}