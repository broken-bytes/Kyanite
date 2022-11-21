import Foundation

public struct Vector3 {
    public private(set) var x: Float
    public private(set) var y: Float
    public private(set) var z: Float

    public init(x: Float, y: Float, z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }
}

public func add(left: Vector3, right: Vector3) -> Vector3 {
    return Vector3(x: left.x + right.x, y: left.y + right.y, z: left.z + right.z)
}

public func sub(left: Vector3, right: Vector3) -> Vector3 {
    return Vector3(x: left.x - right.x, y: left.y - right.y, z: left.z - right.z)
}

public func mul(vector: Vector3, value: Float) -> Vector3 {
    return Vector3(x: vector.x * value, y: vector.y * value, z: vector.z * value)
}

public func div(vector: Vector3, value: Float) -> Vector3 {
    return Vector3(x: vector.x / value, y: vector.y / value, z: vector.z / value)
}

public func lerp(from vector: Vector3, to target: Vector3, value: Float) -> Vector3 {
    return add(left: mul(vector: target, value: value), right: mul(vector: vector, value: (1 - value)))
}

public func mag(vector: Vector3) -> Float {
    return pow(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, 0.5)
}

public func vec3ToVec2(vector: Vector3) -> Vector2 {
    return Vector2(x: vector.x, y: vector.y)
}

public func vec3ToVec4(vector: Vector3) -> Vector4 {
    return Vector4(x: vector.x, y: vector.y, z: vector.z, w: 1)
}

public func vec3ToQuaternion(vector: Vector3) -> Quaternion {
    return Quaternion(qx: vector.x * sin(0.5), qy: vector.y * sin(0.5), qz: vector.z * sin(0.5), qw: cos(0.5))
}

public extension Vector3 {
    static var zero: Vector3 { Vector3(x: 0, y: 0, z: 0) }
    static var one: Vector3 { Vector3(x: 1, y: 1, z: 1) }
}

public class Vector3C {
    public var x: Float
    public var y: Float
    public var z: Float

    public init(x: Float, y: Float, z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }

    public convenience init(vector: Vector2) {
        self.init(x: vector.x, y: vector.y, z: 1)
    }

    public convenience init(vector: Vector3) {
        self.init(x: vector.x, y: vector.y, z: vector.z)
    }

    public static func +(lhs: Vector3C, rhs: Vector3C) -> Vector3C {
        return Vector3C(x: lhs.x + rhs.x, y: lhs.y + rhs.y, z: lhs.z + rhs.z)
    }

    public static func *(lhs: Vector3C, rhs: Vector3C) -> Vector3C {
        return Vector3C(x: lhs.x * rhs.x, y: lhs.y * rhs.y, z: lhs.z * rhs.z)
    }

    public static func *(lhs: Vector3C, scalar: Float) -> Vector3C {
        return Vector3C(x: lhs.x * scalar, y: lhs.y * scalar, z: lhs.z * scalar)
    }

    public static func /(lhs: Vector3C, scalar: Float) -> Vector3C {
        return Vector3C(x: lhs.x / scalar, y: lhs.y / scalar, z: lhs.z / scalar)
    }

    public func magnitude() -> Float {
        return pow(self.x * self.x + self.y * self.y + self.z * self.z, 0.5)
    }
}