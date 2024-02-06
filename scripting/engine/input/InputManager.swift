import Native

public class InputManager {
    public static let shared = InputManager()

    private init() {
    }

    internal func update() {
        NativeInput.shared.update()
    }

    public func getKeyState(key: KeyboardKey) -> ButtonState {
        let buttonId = NativeInput.shared.getKeyState(key: key.rawValue)

        guard let button = ButtonState(rawValue: buttonId) else {
            fatalError("Invalid button state")
        }

        return button
    }
}