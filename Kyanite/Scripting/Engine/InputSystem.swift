public protocol InputEvent {
    
}

public class InputSystem: EventSystem<InputEvent> {
    public enum ButtonState {
        case none // When button is not held 
        case pressed // When button was pressed this frame
        case released // When button was released this frame
        case held // When Button is held
    }

    public enum MouseButton: UInt8 {
        case first
        case second
        case third
        case fourth
        case fifth
        case sixth
    }
    
    public static let shared = InputSystem()

    // MARK: Private state management props

    private var mouseButtonStates: [MouseButton : ButtonState] = [:]

    private override init() {
        mouseButtonStates[.first] = .none
        mouseButtonStates[.second] = .none
        mouseButtonStates[.third] = .none
        mouseButtonStates[.fourth] = .none
        mouseButtonStates[.fifth] = .none
        mouseButtonStates[.sixth] = .none
    }

    internal func flush() {
        // Reset the Input each frame. 
        // - All events are cleared
        // - Pressed and released messages removed and state is changed
        
        // Check what state the mouse buttons are in 
        // - If the state is pressed that means we dit not get any release message, thus the button must still be held
        // - If the state is released that means we dit not get any pressed message, thus the button must still be released, thus none
        for mbState in mouseButtonStates {
            if mbState.value == .pressed {
                mouseButtonStates[mbState.key] = .held
            }
            if mbState.value == .released {
                mouseButtonStates[mbState.key] = .none
            }
        }
    }

    public func mouseButtonState(for button: MouseButton) -> ButtonState {
        guard let btn = mouseButtonStates[button] else { fatalError("Failed to read mouse data") }
        return btn
    }

    internal func setMouseButton(button: MouseButton, isPressed: Bool) {
        mouseButtonStates[button] = isPressed ? .pressed : .released
    }
}