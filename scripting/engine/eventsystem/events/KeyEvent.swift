public struct KeyEvent: Event {
    public var type: Any.Type { KeyEvent.self }
    public var key: KeyboardKey
    public var mods: Int
    public var state: ButtonState

    public init(key: KeyboardKey, state: ButtonState, modifiers: Int) {
        self.key = key
        self.mods = modifiers
        self.state = state
    }
}