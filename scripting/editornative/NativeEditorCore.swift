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

    public func beginWindow(title: String, flags: Int32, id: Int64, onClose: @convention(c) (Int64) -> Void) {
        EditorCore_BeginWindow(title, flags, id, onClose)
    }

    public func endWindow() {
        return EditorCore_EndWindow()
    }

    public func button(_ label: String) -> Bool {
        return EditorCore_Button(label.cString(using: .utf8))
    }

    public func label(_ text: String) {
        return EditorCore_Label(text.cString(using: .utf8))
    }

    public func drawDefaultImGuiDockspace() {
        EditorCore_DrawDefaultDocking()
    }
}