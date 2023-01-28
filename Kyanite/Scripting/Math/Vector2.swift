import Foundation

public struct Vector2 {
    public private(set) var x: Float
    public private(set) var y: Float
    
    public init(x: Float, y: Float) {
        self.x = x
        self.y = y
    }
}

public func add(left: Vector2, right: Vector2) -> Vector2 {
    return Vector2(x: left.x + right.x, y: left.y + right.y)
}

public func sub(left: Vector2, right: Vector2) -> Vector2 {
    return Vector2(x: left.x - right.x, y: left.y - right.y)
}

public func mul(vector: Vector2, value: Float) -> Vector2 {
    return Vector2(x: vector.x * value, y: vector.y * value)
}

public func div(vector: Vector2, value: Float) -> Vector2 {
    return Vector2(x: vector.x / value, y: vector.y / value)
}

public func lerp(from vector: Vector2, to target: Vector2, value: Float) -> Vector2 {
    return add(left: mul(vector: target, value: value), right: mul(vector: vector, value: (1 - value)))
}

public func mag(vector: Vector2) -> Float {
    return pow(vector.x * vector.x + vector.y * vector.y, 0.5)
}

public func vec2ToVec3(vector: Vector2) -> Vector3 {
    return Vector3(x: vector.x, y: vector.y, z: 1)
}

public func vec2ToVec4(vector: Vector2) -> Vector4 {
    return Vector4(w: 1, x: vector.x, y: vector.y, z: 1)
}

public extension Vector2 {
    static var zero: Vector2 { return Vector2(x: 0, y: 0) }
    static var one: Vector2 { return Vector2(x: 1, y: 1) }
}

public class Vector2C {
    public var x: Float
    public var y: Float

    public init(x: Float, y: Float) {
        self.x = x
        self.y = y
    }

    public convenience init(vector: Vector2) {
        self.init(x: vector.x, y: vector.y)
    }

    public static func +(lhs: Vector2C, rhs: Vector2C) -> Vector2C {
        return Vector2C(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
    }

    public static func *(lhs: Vector2C, rhs: Vector2C) -> Vector2C {
        return Vector2C(x: lhs.x * rhs.x, y: lhs.y * rhs.y)
    }

    public static func *(lhs: Vector2C, scalar: Float) -> Vector2C {
        return Vector2C(x: lhs.x * scalar, y: lhs.y * scalar)
    }

    public static func /(lhs: Vector2C, scalar: Float) -> Vector2C {
        return Vector2C(x: lhs.x / scalar, y: lhs.y / scalar)
    }

    public func magnitude() -> Float {
        return pow(self.x * self.x + self.y * self.y, 0.5)
    }
}