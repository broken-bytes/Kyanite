import Foundation
import Core
import Math

#if _EDITOR
import Editor
#endif

internal class Engine {
    #if _EDITOR
    private let editor = Editor()
    #endif
    private var deltaTime: Float = 0
    private var cpuTime: Float = 0
    private var gpuTime: Float = 0    

    internal init() {
    }

    internal func start(
        args: [String],
        width: UInt32, 
        height: UInt32, 
        window: UnsafeMutableRawPointer
    ) {
        args[1].withCString {
            NativeCore.shared.start(width: width, height: height, window: window, rootDir: $0)
        }

        #if _EDITOR
        editor.start()
        #endif
    }
    
    internal func update() {
        #if _EDITOR
        editor.update()
        #endif
        var start = Date()
        NativeCore.shared.update(tick: deltaTime)
        InputSystem.shared.flush()
        NativeCore.shared.endUpdate()
        var delta: TimeInterval = start.distance(to: Date())
        Timing.shared.deltaTime = Float(delta)
    }

    internal func onKeyChanged(key: UInt8, isPressed: Bool, name: String) {
        guard let keyData = InputSystem.KeyboardButton(rawValue: key) else { return }
        InputSystem.shared.setKeyboardButton(button: keyData, isPressed: isPressed, name: name)
    }

    internal func onMouseButtonChanged(button: UInt8, isPressed: Bool) {
        guard let keyData = InputSystem.MouseButton(rawValue: button) else { return }
        InputSystem.shared.setMouseButton(button: keyData, isPressed: isPressed)
    }

    internal func onAxisChanged(axis: UInt8, value: Float) {

    }

    internal func onMousePositionChanged(mouseX: Int32, mouseY: Int32) {
        InputSystem.shared.setMouseMoved(x: mouseX, y: mouseY)
    }

    internal func onViewportResized(width: UInt32, height: UInt32) {
        NativeCore.shared.setResized(width: width, height: height)
    }
}
