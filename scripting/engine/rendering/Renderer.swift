public class Renderer {
    public static let shared = Renderer()

    private init() {
    }

    public func render() {
        Rendering_Render()
    }

    public func update(with deltaTime: Float) {
        Rendering_Update(deltaTime)
    }
}