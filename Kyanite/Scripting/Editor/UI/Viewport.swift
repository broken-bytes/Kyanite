import Core

private typealias RenderStart = (UnsafeMutableRawPointer) -> Void

public struct Viewport: EditorWindow {
    public var width: Float = 0
    public var height: Float = 0
    public var foreground: Bool = false
    public var title: String = "Viewport"
    
    private var renderUpdate: ((Float) -> Void)!

    public init(hwnd: UnsafeMutableRawPointer) {
        print(hwnd)
        let lib = Library.loadLibrary(at: "./Kyanite.dll")
        print(lib)
        var startFunc: RenderStart = lib.loadFunc(named: "start")
        print(startFunc)
        startFunc(hwnd)
        
    }

    public func draw() {
    
    }
}