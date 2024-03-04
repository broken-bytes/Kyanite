import KyaniteEngine

class EditorEnvironment {
    static let shared = EditorEnvironment()

    var editorParent: Entity!
    var selectedObject: UInt64? = nil

    func configure() {
        editorParent = Entity(name: "Editor")
    }
}