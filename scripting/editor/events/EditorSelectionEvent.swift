import KyaniteEngine

struct EditorSelectionEvent: Event {
    var type: Any.Type { EditorSelectionEvent.self }
    let selectedObject: Entity?
}