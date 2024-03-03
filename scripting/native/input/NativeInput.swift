@_implementationOnly import Bridge
import Foundation

public class NativeInput {
    public enum KeyEventType: Int32, RawRepresentable {
        case pressed = 1
        case released = 0
    }

    public struct NativeKeyEvent {
        public let type: KeyEventType;
        public let scancode: UInt32;
        public let mods: Int
    }

    public static let shared = NativeInput()
    private static var eventSubscriber: ((NativeKeyEvent) -> Void)? = nil

    public func start(imGui: UnsafeMutableRawPointer) {
        Input_Init(imGui)
    }

    public func update() {
        Input_Poll()
    }

    public func subscribeToInputs(block: @escaping (NativeKeyEvent) -> Void) {
        NativeInput.eventSubscriber = block
        Input_Subscribe { event in 
            guard let event = event?.pointee else { return }
            // Handle the event
            switch event.type {
                case Key:
                    let keyboardEvent = event.data.key
                    let keyEvent = NativeKeyEvent(
                        type: KeyEventType(rawValue: keyboardEvent.type.rawValue)!, 
                        scancode: keyboardEvent.scancode, 
                        mods: Int(keyboardEvent.mods)
                    )
                    NativeInput.eventSubscriber?(keyEvent)
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
}