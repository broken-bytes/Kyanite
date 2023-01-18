public class InputSystem: EventSystem<InputEvent> {
    public enum ButtonState {
        case none // When button is not held 
        case pressed // When button was pressed this frame
        case released // When button was released this frame
        case held // When Button is held
    }

    public enum MouseButton: UInt8 {
        case left = 1
        case middle
        case right
        case thumb1
        case thumb2
    }

    public enum KeyboardButton : UInt8, CaseIterable {
        case a = 4
        case b = 5
        case c = 6
        case d = 7
        case e = 8
        case f = 9
        case g = 10
        case h = 11
        case i = 12
        case j = 13
        case k = 14
        case l = 15
        case m = 16
        case n = 17
        case o = 18
        case p = 19
        case q = 20
        case r = 21
        case s = 22
        case t = 23
        case u = 24
        case v = 25
        case w = 26
        case x = 27
        case y = 28
        case z = 29
        case alpha1 = 30
        case alpha2 = 31
        case alpha3 = 32
        case alpha4 = 33
        case alpha5 = 34
        case alpha6 = 35
        case alpha7 = 36
        case alpha8 = 37
        case alpha9 = 38
        case alpha0 = 39
        case `return` = 40
        case escape = 41
        case backspace = 42
        case tab = 43
        case space = 44
        case minus = 45
        case equals = 46
        case leftBracket = 47
        case rightBracket = 48
        case backslash = 49
        case semicolon = 51
        case grave = 53
        case comma = 54
        case period = 55
        case slash = 56
        case caps = 57
        case f1 = 58
        case f2 = 59
        case f3 = 60
        case f4 = 61
        case f5 = 62
        case f6 = 63
        case f7 = 64
        case f8 = 65
        case f9 = 66
        case f10 = 67
        case f11 = 68
        case f12 = 69
        case print = 70
        case scrollLock = 71
        case pause = 72
        case insert = 73
        case home = 74
        case pageUp = 75
        case delete = 76
        case end = 77
        case pageDown = 78
        case arrowRight = 79
        case arrowLeft = 80
        case arrowDown = 81
        case arrowUp = 82
        case numLockClear = 83
        case keypadDivide = 84
        case keypadMultiply = 85
        case keypadMinus = 86
        case keypadPlus = 87
        case keypadEnter = 88
        case keypad1 = 89
        case keypad2 = 90
        case keypad3 = 91
        case keypad4 = 92
        case keypad5 = 93
        case keypad6 = 94
        case keypad7 = 95
        case keypad8 = 96
        case keypad9 = 97
        case keypad0 = 98
        case keypadPeriod = 99
        case keypadEquals = 103
        case f13 = 104
        case f14 = 105
        case f15 = 106
        case f16 = 107
        case f17 = 108
        case f18 = 109
        case f19 = 110
        case f20 = 111
        case f21 = 112
        case f22 = 113
        case f23 = 114
        case f24 = 115
        case menu = 118    /**< Menu (show menu) */
        case keypadComma = 133
        case clear = 156
        case separator = 159
        case leftControl = 224
        case leftShift = 225
        case leftAlt = 226 /**< alt option */
        case LeftMeta = 227 /**< windows command (apple) meta */
        case RightControl = 228
        case RightShift = 229
        case RightAlt = 230
        case RightMeta = 231
    }

    public struct MouseMovement {
        let x: Int32
        let y: Int32
    }
    
    public static let shared = InputSystem()

    // MARK: Private state management props

    private var mouseButtonStates: [MouseButton : ButtonState] = [:]
    private var keyboardButtonStates: [KeyboardButton : ButtonState] = [:]
    private var lastMousePos: MouseMovement = MouseMovement(x: 0, y: 0)
    private var mouseMovement: MouseMovement = MouseMovement(x: 0, y: 0)

    private override init() {
        mouseButtonStates[.left] = .none
        mouseButtonStates[.middle] = .none
        mouseButtonStates[.right] = .none
        mouseButtonStates[.thumb1] = .none
        mouseButtonStates[.thumb2] = .none

        for key in KeyboardButton.allCases {
            keyboardButtonStates[key] = .none
        }
    }

    public override func flush() {
        super.flush()
        // Reset the Input each frame. 
        // - All events are cleared
        // - Pressed and released messages removed and state is changed

        // Pass the inputs to IMGUI before clearing
        NativeCore.shared.setMouseMoved(x: lastMousePos.x, y: lastMousePos.y)

        for mbState in mouseButtonStates {
            if mbState.value == .pressed {
                NativeCore.shared.setMouseDown(button: mbState.key.rawValue - 1)
            }
            if mbState.value == .released {
                NativeCore.shared.setMouseUp(button: mbState.key.rawValue - 1)
            }
        }


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

        for kbState in keyboardButtonStates {
            if kbState.value == .pressed {
                keyboardButtonStates[kbState.key] = .held
            }
            if kbState.value == .released {
                keyboardButtonStates[kbState.key] = .none
            }
        }

        mouseMovement = MouseMovement(x: 0, y: 0)
    }

    public func mouseButtonState(for button: MouseButton) -> ButtonState {
        guard let btn = mouseButtonStates[button] else { return .none }
        return btn
    }

    public func buttonState(for button: KeyboardButton) -> ButtonState {
        guard let btn = keyboardButtonStates[button] else { return .none }
        return btn
    }

    internal func setMouseButton(button: MouseButton, isPressed: Bool) {
        mouseButtonStates[button] = isPressed ? .pressed : .released
        self.push(event: MouseInputEvent(button: button, isPressed: isPressed))
    }

    internal func setKeyboardButton(button: KeyboardButton, isPressed: Bool, name: String) {
        keyboardButtonStates[button] = isPressed ? .pressed : .released
        self.push(event: KeyboardInputEvent(button: button, isPressed: isPressed, name: name))
    }

    internal func setMouseMoved(x: Int32, y: Int32) {
        mouseMovement = MouseMovement(x: x - mouseMovement.x, y: y - mouseMovement.y)
        lastMousePos = MouseMovement(x: x, y: y)
    }
}