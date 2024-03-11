import Foundation
@_implementationOnly import Native
import KyaniteEngine

public class EditorWindow {
    public var title: String { "Editor" }
    private var open: Bool
    internal var entity: Entity?

    internal init() {
        open = true
        entity = Entity(name: "Window \(title)", parent: EditorEnvironment.default.editorParent)
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
        Thread { [weak self] in
            var windowComponent = WindowComponent()
            windowComponent.id = self.hashValue
            windowComponent.isOpen = false
            self?.entity?.setComponent(&windowComponent)
        }.start()
    }

    public func onDraw() {

    }

    public func floatField(_ label: String, value: UnsafeMutablePointer<Float>) {
        NativeImGui.shared.floatField(label, value: value)
    }

    public func float2Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        NativeImGui.shared.float2Field(label, value: value)
    }

    public func float3Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        NativeImGui.shared.float3Field(label, value: value)
    }

    public func float4Field(_ label: String, value: UnsafeMutablePointer<Float>) {
        NativeImGui.shared.float4Field(label, value: value)
    }

    public func intField(_ label: String, value: UnsafeMutablePointer<Int32>) {
        NativeImGui.shared.intField(label, value: value)
    }

    internal func onBeginDraw() {
        NativeImGui.shared.beginWindow(title: title, flags: 0, id: Int64(self.hashValue)) { id in 
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
        NativeImGui.shared.endWindow()     
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