import Rendering

public struct SpriteComponent: Component {
    public let sprite: Sprite

    public init(sprite: Sprite) {
        self.sprite = sprite
    }
}