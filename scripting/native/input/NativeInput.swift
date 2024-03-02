@_implementationOnly import Bridge
import Foundation

fileprivate var keyEvents: [KeyEvent] = []
fileprivate var mutex: NSLock = NSLock()
public class NativeInput {
    public static let shared = NativeInput()
    private var keyStates: [UInt32: UInt8] = [:]

    public func start(imGui: UnsafeMutableRawPointer) {
        Input_Init(imGui)
        Input_Subscribe { event in 
            guard let event = event?.pointee else { return }
            // Handle the event
            mutex.lock()
            defer { mutex.unlock() }
            switch event.type {
                case Key:
                    let keyboardEvent = event.data.key
                    keyEvents.append(keyboardEvent)
                case System:
                    let systemEvent = event.data.system
                    if systemEvent.type == Quit {
                        exit(0)
                    }
                default:
                    break
            }
            
        }
    }

    public func update() {
        Input_Poll()
        mutex.lock()
        defer { mutex.unlock() }
        // First, set all keys that were released the last frame to none
        for (key, state) in keyStates {
            if state == 3 {
                keyStates[key] = 0
            }
        }

        // After each poll we need to check the current state of the keys and check if they are still being pressed
        for event in keyEvents {
            // The key is released
            if event.type == KeyReleased {
                keyStates[event.scancode] = 3
            } 
            // The key is pressed
            else if event.type == KeyPressed {
                // If the key was pressed the last frame, we set it to 2(Held)
                keyStates[event.scancode] = keyStates[event.scancode] == 1 ? 2 : 1
            }
        }

        keyEvents.removeAll()
    }

    public func getKeyState(key: UInt32) -> UInt8 {
        return keyStates[key] ?? 0
    }
}