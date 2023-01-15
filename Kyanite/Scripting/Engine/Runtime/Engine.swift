import Foundation
import Core

internal class Engine {
    internal init() {
    }

    internal func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, ctx: UnsafeMutableRawPointer, style: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
        NativeCore.shared.start(width: width, height: height, window: window, ctx: ctx, style: style, rootDir: rootDir)
        let entity = Entity(name: "Test")
        entity.addComponent(component: ColliderComponent())
        InputSystem.shared.subscribe { event in 
            if event is MouseInputEvent {
                
            }
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
