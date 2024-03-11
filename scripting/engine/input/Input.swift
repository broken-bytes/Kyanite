@_implementationOnly import Native

public class Input {
    public static let shared = Input()
    private var lastKeyEvents: [NativeInput.NativeKeyEvent] = []
    private var keyEvents: [NativeInput.NativeKeyEvent] = []

    private init() {
        NativeInput.shared.subscribeToInputs { event in 
            self.keyEvents.append(event)
            guard let key = KeyboardKey(rawValue: event.scancode) else {
                return
            }
            // Also forward the event to the event system
            switch event.type {
                case .pressed:
                    EventSystem.shared.emit(
                        KeyEvent(key: key, state: .pressed, modifiers: event.mods)
                    )
                case .released:
                    EventSystem.shared.emit(
                        KeyEvent(key: key, state: .released, modifiers: event.mods)
                    )
            }
        }
    }

    internal func update() {
        lastKeyEvents = keyEvents
        keyEvents = []

        NativeInput.shared.update()
    }

    public func when(_ key: KeyboardKey, is state: ButtonState, block: @escaping () -> Void) {
        let lastEvents = lastKeyEvents.filter { $0.scancode == key.rawValue }
        let events = keyEvents.filter { $0.scancode == key.rawValue }

        var shouldEmit = false

        // Check if the button was pressed. Otherwise, return
        if 
            state == .pressed && lastEvents.first(where: { $0.type == .pressed }) == nil && 
            events.first(where: { $0.type == .pressed }) != nil 
        {
            shouldEmit = true
        }

        // Check if the button was released. Otherwise, return
        if 
            state == .released && lastEvents.first(where: { $0.type == .released }) == nil && 
            events.first(where: { $0.type == .released }) != nil 
        {
            shouldEmit = true
        }

        // Check if the button is held down. Otherwise, return
        if 
            state == .held && lastEvents.first(where: { $0.type == .pressed }) != nil && 
            events.first(where: { $0.type == .pressed }) != nil 
        {
            shouldEmit = true
        }

        // Check if the button is none. Otherwise, return
        if 
            state == .none && lastEvents.first(where: { $0.type == .pressed }) == nil && 
            events.first == nil 
        {
            shouldEmit = true
        }

        if shouldEmit {
            block()
        }
    }
}