import Foundation
@_implementationOnly import Bridge

public class EditorNativeCore {
    public static let shared = EditorNativeCore()

    private init() {}

    public func start(imGuiContext: UnsafeMutableRawPointer) {
        EditorCore_Init(imGuiContext)
    }

    deinit {
        // TODO: Implement EditorCore_Shutdown
    }

    public func beginWindow(title: String, open: inout Bool, flags: Int32) {
        return EditorCore_BeginWindow(title.cString(using: .utf8), &open, flags)
    }

    public func endWindow() {
        return EditorCore_EndWindow()
    }

    public func drawDefaultImGuiDockspace() {
        EditorCore_DrawDefaultDocking()
    }
}