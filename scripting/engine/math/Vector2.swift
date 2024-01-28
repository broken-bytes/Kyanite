public struct Vec2<T> where T: Hashable, T: Comparable, T: Numeric {
    public var x: T
    public var y: T

    public static var zero: Vec2<T> { get { Vec2(x: 0, y: 0) } }

    public init(x: T, y: T) {
        self.x = x
        self.y = y
    }
}

typealias Vector2 = Vec2<Float>

extension Vec2: Hashable {
    public static func == (lhs: Vec2<T>, rhs: Vec2<T>) -> Bool {
        return lhs.x == rhs.x && lhs.y == rhs.y
    }
}