import Foundation
@_implementationOnly import Bridge

public class NativeCore {
    public static let shared = NativeCore()

    private init() {}

    public func start() {
        Core_Init()
    }

    deinit {
        Core_Shutdown()
    }

    public func createWindow(
        _ name: String, 
        posX: UnsafeMutablePointer<UInt32>?, 
        posY: UnsafeMutablePointer<UInt32>?, 
        width: UInt32, 
        height: UInt32,
        flags: UInt32,
        backend: UInt8,
        silent: Bool
    ) -> NativeWindow {
        return NativeWindow(
            handle: Core_CreateWindow(
                name.cString(using: .utf8),
                posX,
                posY,
                width,
                height,
                flags,
                backend,
                silent
            )
        )
    }
}