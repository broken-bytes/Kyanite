import Foundation
import Native
import EditorNative
import KyaniteEngine
import WinSDK

public class EditorWindow {
    public var title: String { "Editor" }
    private var open: Bool
    internal var entity: Entity?

    internal init() {
        open = true
        OutputDebugStringA("EditorWindow creating\n")
        entity = Entity(name: "Window \(title)", parent: EditorEnvironment.shared.editorParent)
        OutputDebugStringA("EditorWindow created\n")

        entity?.addComponent(WindowComponent.self)
        show()
    }

    public func show() {
        var windowComponent = WindowComponent()
        windowComponent.id = self.hashValue
        windowComponent.isOpen = true
        entity?.setComponent(&windowComponent)
    }

    public func hide() {
        // Run this on another thread in case the callback is run when the ecs write lock is held
        var workerThread = Thread { [weak self] in
            var windowComponent = WindowComponent()
            windowComponent.id = self.hashValue
            windowComponent.isOpen = false
            self?.entity?.setComponent(&windowComponent)
        }.start()
    }

    public func onDraw() {

    }

    internal func onBeginDraw() {
        EditorNativeCore.shared.beginWindow(title: title, flags: 0, id: Int64(self.hashValue)) { id in 
            guard 
                let window = WindowManager.shared.windows.first(
                    where: { $0.hashValue == id }
                ) 
            else {
                return
            }

            window.hide()
        }
    }
    
    internal func onEndDraw() {
        EditorNativeCore.shared.endWindow()     
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