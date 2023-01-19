import Math
import Engine

public struct MoveComponent: Component {
    public var movement: Vector3

    public init(movement: Vector3) {
        self.movement = movement
    }
}