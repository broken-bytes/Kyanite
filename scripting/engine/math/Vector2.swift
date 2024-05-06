public struct Vec2<T>: Codable where T: Hashable, T: Comparable, T: Numeric, T: Codable {
    public var x: T
    public var y: T

    public static var zero: Vec2<T> { get { Vec2(x: 0, y: 0) } }

    public init(x: T, y: T) {
        self.x = x
        self.y = y
    }

    public init(_ x: T) {
        self.x = x
        self.y = x
    }

    public init(_ value: (T, T)) {
        self.x = value.0
        self.y = value.1
    }
}

public typealias Vector2 = Vec2<Float>

extension Vec2: Hashable {
    public static func == (lhs: Vec2<T>, rhs: Vec2<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y
    }
}