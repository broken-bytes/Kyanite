public class InputManager {
    public static let shared = InputManager()

    private init() {
        Bridge_Iput_Init()
    }

    internal func update() {
        Bridge_Input_Poll()
    }

    public func getKeyState(key: KeyboardKey) -> ButtonState {
        guard let button = ButtonState(rawValue: Bridge_Input_GetKeyboardButtonState(key.rawValue)) else {
            fatalError("Invalid button state")
        }

        return button
    }
}