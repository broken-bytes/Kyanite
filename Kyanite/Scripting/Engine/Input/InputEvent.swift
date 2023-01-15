public protocol InputEvent {
    
}

public struct MouseInputEvent : InputEvent {
    let button: InputSystem.MouseButton
    let isPressed: Bool
}
