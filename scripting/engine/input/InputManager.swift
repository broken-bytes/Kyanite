public class InputManager {
    public static let shared = InputManager()

    private init() {
    }

    internal func update() {
        Input_Poll()
    }

    public func getKeyState(key: KeyboardKey) -> ButtonState {
        guard let button = ButtonState(rawValue: Input_GetKeyboardButtonState(key.rawValue)) else {
            fatalError("Invalid button state")
        }

        return button
    }
}