import Rendering

public struct SpriteComponent: Component {
        // Mesh Instance
    public let sprite: Sprite

    public init(sprite: Sprite) {
        self.sprite = sprite
    }
}