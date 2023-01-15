import Foundation
import Core

internal class Engine {
    internal init() {
    }

    internal func start(
        width: UInt32, 
        height: UInt32, 
        window: UnsafeMutableRawPointer, 
        rootDir: UnsafeMutableRawPointer,
        world: String
    ) {
        NativeCore.shared.start(width: width, height: height, window: window, rootDir: rootDir)
        Task {
            try! await WorldManager.shared.loadWorld(name: world)
        }
    }
    
    internal func update(timestep: Float) {
        NativeCore.shared.update(tick: timestep)
        InputSystem.shared.flush()
    }

    internal func onKeyChanged(key: UInt8, isPressed: Bool, name: String) {
        InputSystem.shared.setKeyboardButton(button: InputSystem.KeyboardButton(rawValue: key)!, isPressed: isPressed, name: name)
    }

    internal func onMouseButtonChanged(button: UInt8, isPressed: Bool) {
        InputSystem.shared.setMouseButton(button: InputSystem.MouseButton(rawValue: button)!, isPressed: isPressed)
    }

    internal func onAxisChanged(axis: UInt8, value: Float) {

    }

    internal func onMousePositionChanged(mouseX: UInt32, mouseY: UInt32) {

    }
}
