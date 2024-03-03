import Native

class DebugConsole: EditorWindow {
    public override var title: String { "Debug Console" }
    private var selectedObject: UInt64? = nil
    
    override init() {
        super.init()
    }

    override func onDraw() {
        if let selectedObject = selectedObject {
            // Draw the inspector window
        }
    }
}