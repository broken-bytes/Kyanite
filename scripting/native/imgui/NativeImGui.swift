import Bridge

public class NativeImGui {
    public static let shared = NativeImGui()

    private init() {}


    public func createContext() -> UnsafeMutableRawPointer {
        return ImGui_CreateImGuiContext()
    }

    public func setContext(_ context: UnsafeMutableRawPointer) {
        ImGui_Init(context)
    }

    public func getContext() -> UnsafeMutableRawPointer {
        return ImGui_GetImGuiContext()
    }

    public func beginWindow(title: String, flags: Int32, id: Int64, onClose: @convention(c) (Int64) -> Void) {
        ImGui_BeginWindow(title, flags, id, onClose)
    }

    public func endWindow() {
        return ImGui_EndWindow()
    }

    public func button(_ label: String) -> Bool {
        return ImGui_Button(label)
    }

    public func label(_ text: String) {
        return ImGui_Label(text)
    }

    public func floatField(_ label: String, value: UnsafeMutablePointer<Float>) {
        return ImGui_FloatField(label, value)
    }

    public func float2Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return ImGui_Float2Field(label, value)
    }

    public func float3Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return ImGui_Float3Field(label, value)
    }

    public func float4Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        return ImGui_Float4Field(label, value)
    }

    public func intField(_ label: String, value: UnsafeMutablePointer<Int32>) {
        return ImGui_IntField(label, value)
    }

    public func checkbox(_ label: String, value: UnsafeMutablePointer<Bool>) {
        return ImGui_Checkbox(label, value)
    }

    public func beginHGroup() {
        return ImGui_BeginHorizontal()
    }

    public func endHGroup() {
        return ImGui_EndHorizontal()
    }

    public func separator() {
        return ImGui_Separator()
    }

    public func drawDefaultImGuiDockspace() {
        ImGui_DrawDefaultDocking()
    }
}