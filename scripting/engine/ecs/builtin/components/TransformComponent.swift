@Component
public struct TransformComponent {
    var position: Vector3 = .zero
    var rotation: Quaternion = .identity
    var scale: Vector3 = .zero

    public init(position: Vector3 = .zero, rotation: Quaternion = .identity, scale: Vector3 = .zero) {
        self.position = position
        self.rotation = rotation
        self.scale = scale
    }
}