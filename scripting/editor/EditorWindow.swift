import Foundation
import Native
import KyaniteEngine

public class EditorWindow {
    public var title: String { "Editor" }
    private var open: Bool = true
    private var entity: Entity? = nil

    internal init() {
        open = true
        entity = Entity(name: "Window \(title)")
        entity?.addComponent(WindowComponent.self)
        var windowComponent = WindowComponent()
        windowComponent.id = self.hashValue
        windowComponent.isOpen = open
        entity?.setComponent(&windowComponent)
    }

    public func show() {
        var windowComponent = WindowComponent()
        windowComponent.id = self.hashValue
        windowComponent.isOpen = true
        entity?.setComponent(&windowComponent)
    }

    public func hide() {
        var windowComponent = WindowComponent()
        windowComponent.id = self.hashValue
        windowComponent.isOpen = false
        entity?.setComponent(&windowComponent)
    }

    public func onDraw() {

    }

    internal func onBeginDraw() {
        NativeRendering.shared.startWindow(title, open: &open)
    }
    
    internal func onEndDraw() {
        NativeRendering.shared.endWindow()        
    }
}

extension EditorWindow: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(title)
    }

    public static func == (lhs: EditorWindow, rhs: EditorWindow) -> Bool {
        return lhs.title == rhs.title
    }
}