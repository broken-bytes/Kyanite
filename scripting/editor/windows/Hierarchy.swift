import Native

class Hierarchy: EditorWindow {
    public override var title: String { "Hierarchy" }
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