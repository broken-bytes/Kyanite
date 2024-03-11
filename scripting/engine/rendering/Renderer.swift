@_implementationOnly import Native

public class Renderer {
    public static let shared = Renderer()

    private init() {
    }

    func preFrame() {
        NativeRendering.shared.preFrame()
    }

    func update(with deltaTime: Float) {
        NativeRendering.shared.update(with: deltaTime)
    }

    func postFrame() {
        NativeRendering.shared.postFrame()
    }
}