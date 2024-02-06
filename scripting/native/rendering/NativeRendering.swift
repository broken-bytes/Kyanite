@_implementationOnly import Bridge

public class NativeRendering {
    public static let shared = NativeRendering()

    private init() {}

    public func start(window: NativeWindow) {
        Rendering_Init(window.handle)
    }

    public func preFrame() {
        Rendering_PreFrame()
    }

    public func update(with deltaTime: Float) {
        Rendering_Update(deltaTime)
    }

    public func postFrame() {
        Rendering_PostFrame()
    }

    public func drawText(_ text: String) {
        Rendering_DrawText(text)
    }

    public func startWindow(_ name: String, open: inout Bool) {
        Rendering_StartWindow(name.cString(using: .utf8), &open)
    }

    public func endWindow() {
        Rendering_EndWindow()
    }
}