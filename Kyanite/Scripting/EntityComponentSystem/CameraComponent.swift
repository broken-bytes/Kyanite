public struct CameraComponent: Component {
    public var fov: Float
    public var nearClipPlane: Float
    public var farClipPlane: Float
    public var clearColor: Color
    public var renderTarget: RenderTarget

    public init() {
        
    }
}