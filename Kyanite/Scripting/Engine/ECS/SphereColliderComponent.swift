import Math

public struct SphereColliderComponent: Component {
    public let radius: Float

    public init(radius: Float) {
        self.radius = radius
    }
}