import Foundation

public struct Vector4 {
    public private(set) var w: Float
    public private(set) var x: Float
    public private(set) var y: Float
    public private(set) var z: Float
    
    
    public init( w: Float, x: Float, y: Float, z: Float) {
        self.w = w
        self.x = x
        self.y = y
        self.z = z
    }
}

public func add(left: Vector4, right: Vector4) -> Vector4 {
    return Vector4(w: left.w + right.w, x: left.x + right.x, y: left.y + right.y, z: left.z + right.z)
}

public func sub(left: Vector4, right: Vector4) -> Vector4 {
    return Vector4(w: left.w - right.w, x: left.x - right.x, y: left.y - right.y, z: left.z - right.z)
}

public func mul(vector: Vector4, value: Float) -> Vector4 {
    return Vector4(w: vector.w * value, x: vector.x * value, y: vector.y * value, z: vector.z * value)
}

public func div(vector: Vector4, value: Float) -> Vector4 {
    return Vector4(w: vector.w / value, x: vector.x / value, y: vector.y / value, z: vector.z / value)
}

public func lerp(from vector: Vector4, to target: Vector4, value: Float) -> Vector4 {
    return add(left: mul(vector: target, value: value), right: mul(vector: vector, value: (1 - value)))
}

public func mag(vector: Vector4) -> Float {
    return pow(vector.w * vector.w + vector.x * vector.x + vector.y * vector.y + vector.z * vector.z, 0.5)
}

public func vec4ToVec2(vector: Vector4) -> Vector2 {
    return Vector2(x: vector.x, y: vector.y)
}

public func vec4ToVec3(vector: Vector4) -> Vector3 {
    return Vector3(x: vector.x, y: vector.y, z: vector.z)
}

public extension Vector4 {
    static var zero: Vector4    { return Vector4(w: 0, x: 0, y: 0, z: 0) }
    static var one: Vector4     { return Vector4(w: 1, x: 1, y: 1, z: 1) }
}
