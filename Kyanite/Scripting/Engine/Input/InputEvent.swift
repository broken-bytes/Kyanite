public protocol InputEvent {
    
}

struct MouseInputEvent : InputEvent {
    let button: InputSystem.MouseButton
    let isPressed: Bool
}
