import KyaniteEngine

class EditorEnvironment {
    static let shared = EditorEnvironment()

    var editorParent: Entity!

    func configure() {
        editorParent = Entity(name: "Editor")
    }
}