import Foundation

public struct Vector4 {
    public private(set) var x: Float
    public private(set) var y: Float
    public private(set) var z: Float
    public private(set) var w: Float
    
    
    public init(x: Float, y: Float, z: Float, w: Float) {
        self.x = x
        self.y = y
        self.z = z
        self.w = w
    }
}

public func add(left: Vector4, right: Vector4) -> Vector4 {
    return Vector4(x: left.x + right.x, y: left.y + right.y, z: left.z + right.z, w: left.w + right.w)
}

public func sub(left: Vector4, right: Vector4) -> Vector4 {
    return Vector4(x: left.x - right.x, y: left.y - right.y, z: left.z - right.z, w: left.w - right.w)
}

public func mul(vector: Vector4, value: Float) -> Vector4 {
    return Vector4(x: vector.x * value, y: vector.y * value, z: vector.z * value, w: vector.w)
}

public func div(vector: Vector4, value: Float) -> Vector4 {
    return Vector4(x: vector.x / value, y: vector.y / value, z: vector.z / value, w: vector.w / value)
}

public func lerp(from vector: Vector4, to target: Vector4, value: Float) -> Vector4 {
    return add(left: mul(vector: target, value: value), right: mul(vector: vector, value: (1 - value)))
}

public func mag(vector: Vector4) -> Float {
    return pow(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z + vector.w * vector.w, 0.5)
}

public func vec4ToVec2(vector: Vector4) -> Vector2 {
    return Vector2(x: vector.x, y: vector.y)
}

public func vec4ToVec3(vector: Vector4) -> Vector3 {
    return Vector3(x: vector.x, y: vector.y, z: vector.z)
}

public extension Vector4 {
    static var zero: Vector4 { return Vector4(x: 0, y: 0, z: 0, w: 0) }
    static var one: Vector4 { return Vector4(x: 1, y: 1, z: 1, w: 1) }
}

public class Vector4C {
    public var x: Float
    public var y: Float
    public var z: Float
    public var w: Float

    public init(x: Float, y: Float, z: Float, w: Float) {
        self.x = x
        self.y = y
        self.z = z
        self.w = w
    }

    public convenience init(vector: Vector2) {
        self.init(x: vector.x, y: vector.y, z: 1, w: 1)
    }

    public convenience init(vector: Vector3) {
        self.init(x: vector.x, y: vector.y, z: vector.z, w: 1)
    }

    public convenience init(vector: Vector4) {
        self.init(x: vector.x, y: vector.y, z: vector.z, w: vector.w)
    }

    public static func +(lhs: Vector4C, rhs: Vector4C) -> Vector4C {
        return Vector4C(x: lhs.x + rhs.x, y: lhs.y + rhs.y, z: lhs.z + rhs.z, w: lhs.w + rhs.w)
    }

    public static func *(lhs: Vector4C, rhs: Vector4C) -> Vector4C {
        return Vector4C(x: lhs.x * rhs.x, y: lhs.y * rhs.y, z: lhs.z * rhs.z, w: lhs.w * rhs.w)
    }

    public static func *(lhs: Vector4C, scalar: Float) -> Vector4C {
        return Vector4C(x: lhs.x * scalar, y: lhs.y * scalar, z: lhs.z * scalar, w: lhs.w * scalar)
    }

    public static func /(lhs: Vector4C, scalar: Float) -> Vector4C {
        return Vector4C(x: lhs.x / scalar, y: lhs.y / scalar, z: lhs.z / scalar, w: lhs.w / scalar)
    }

    public func magnitude() -> Float {
        return pow(self.x * self.x + self.y * self.y + self.z * self.z + self.w * self.w, 0.5)
    }
}