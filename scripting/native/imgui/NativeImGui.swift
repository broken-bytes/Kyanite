@_implementationOnly import Bridge

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

    public func beginContextWindow() -> Bool {
        return ImGui_BeginPopupContextWindow()
    }

    public func endPopup() {
        return ImGui_EndPopup()
    }

    public func beginMenu(_ label: String) -> Bool {
        return ImGui_BeginMenu(label)
    }

    public func endMenu() {
        return ImGui_EndMenu()
    }

    public func menuItem(_ label: String) -> Bool {
        return ImGui_MenuItem(label)
    }

    public func getCursorPos(_ y: inout Float) {
        ImGui_GetCursorPos(&y)
    }

    public func setCursorPos(_ y: Float) {
        return ImGui_SetCursorPos(y)
    }

    public func sameLine() {
        return ImGui_SameLine()
    }

    public func dummy(_ width: Float, _ height: Float) {
        return ImGui_Dummy(width, height)
    }

    public func button(_ label: String) -> Bool {
        return ImGui_Button(label, nil, nil)
    }

    public func button(_ label: String, width: inout Float, height: inout Float) -> Bool {
        return ImGui_Button(label, &width, &height)
    }

    public func label(_ text: String) {
        return ImGui_Label(text)
    }

    public func beginTable(_ name: String, count: Int32, flags: Int32) -> Bool {
        return ImGui_BeginTable(name, count, flags)
    }

	public func endTable() {
        return ImGui_EndTable()
    }

	public func setupColumn(_ label: String) {
        return ImGui_SetupColumn(label)
    }

	public func tableNextRow() {
        return ImGui_TableNextRow()
    }

	public func tableSetColumnIndex(_ index: Int32) {
        return ImGui_TableSetColumnIndex(index)
    }

    public func nextColumn() {
        return ImGui_NextColumn()
    }

    public func getAvailableWidth() -> Float {
        return ImGui_GetAvailableWidth()
    }

    public func icon(_ icon: String) {
        return ImGui_Icon(icon)
    }

    public func text(_ label: String, text: UnsafeMutableBufferPointer<UInt8>) -> Bool {
        ImGui_TextField(label, text.baseAddress, Int32(text.count))
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

    public func treeNode(_ label: String) -> Bool {
        return ImGui_TreeNode(label)
    }

    public func treePop() {
        return ImGui_TreePop()
    }

    public func drawDefaultImGuiDockspace() {
        ImGui_DrawDefaultDocking()
    }
}