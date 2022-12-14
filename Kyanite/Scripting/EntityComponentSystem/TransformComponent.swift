import Math

public struct TransformComponent: Component {
    public var position: Vector3
    public var scale: Vector3
    public var rotation: Vector3

    public init(position: Vector3, scale: Vector3, rotation: Vector3) {
        self.position = position
        self.scale = scale
        self.rotation = rotation
    }
}