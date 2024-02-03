public class Renderer {
    public static let shared = Renderer()

    private init() {
    }

    func preFrame() {
        Rendering_PreFrame()
    }

    func update(with deltaTime: Float) {
        Rendering_Update(deltaTime)
    }

    func postFrame() {
        Rendering_PostFrame()
    }
}