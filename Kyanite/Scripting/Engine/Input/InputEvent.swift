public protocol InputEvent {
    
}

public struct MouseInputEvent : InputEvent {
    let button: InputSystem.MouseButton
    let isPressed: Bool
}

public struct KeyboardInputEvent : InputEvent {
    let button: InputSystem.KeyboardButton
    let isPressed: Bool
    let name: String
}
