@_implementationOnly import Bridge

public class NativeInput {
    public static let shared = NativeInput()

    public func start() {
        Input_Init()
    }

    public func update() {
        Input_Poll()
    }

    public func getKeyState(key: UInt32) -> UInt8 {
        return Input_GetKeyboardButtonState(key)
    }
}