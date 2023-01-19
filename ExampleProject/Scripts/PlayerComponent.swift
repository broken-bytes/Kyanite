import Engine

public struct PlayerComponent: Component {
    var id: UInt64

    public init(id: UInt64) {
        self.id = id
    }
}