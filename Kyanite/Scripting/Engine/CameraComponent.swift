import Rendering

public struct CameraComponent: Component {
    public var fov: Float
    public var nearClipPlane: Float
    public var farClipPlane: Float
    public var clearColor: Color
    public var renderTarget: RenderTarget

    public init() {
        self.fov = 90
        self.nearClipPlane = 0.1
        self.farClipPlane = 1000
        self.clearColor = .black
        self.renderTarget = RenderTarget()
    }
}