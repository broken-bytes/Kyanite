@_implementationOnly import Bridge

public class NativeRendering {
    public static let shared = NativeRendering()

    private init() {}

    deinit {
        Rendering_Shutdown()
    }

    public func start(window: UnsafeMutableRawPointer, imGui: UnsafeMutableRawPointer) {
        Rendering_Init(window, imGui)
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

    public func loadShader(code: String, type: NativeShaderType) -> UInt64 {
        Rendering_CreateShader(code.cString(using: .utf8), type.rawValue)
    }

    public func unloadShader(handle: UInt64) {
        Rendering_DestroyShader(handle)
    }
}