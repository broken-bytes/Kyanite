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
        return EditorCore_Button(label)
    }

    public func label(_ text: String) {
        return EditorCore_Label(text)
    }

    public func floatField(_ label: String, value: UnsafeMutablePointer<Float>) {
        return EditorCore_FloatField(label, value)
    }

    public func float2Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return EditorCore_Float2Field(label, value)
    }

    public func float3Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return EditorCore_Float3Field(label, value)
    }

    public func float4Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return EditorCore_Float4Field(label, value)
    }

    public func intField(_ label: String, value: UnsafeMutablePointer<Int32>) {
        return EditorCore_IntField(label, value)
    }

    public func checkbox(_ label: String, value: UnsafeMutablePointer<Bool>) {
        return EditorCore_Checkbox(label, value)
    }

    public func beginHGroup() {
        return EditorCore_BeginHorizontal()
    }

    public func endHGroup() {
        return EditorCore_EndHorizontal()
    }

    public func separator() {
        return EditorCore_Separator()
    }

    public func drawDefaultImGuiDockspace() {
        EditorCore_DrawDefaultDocking()
    }
}