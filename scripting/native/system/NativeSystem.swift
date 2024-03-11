@_implementationOnly import Bridge

public class NativeSystem {
    public static let shared = NativeSystem()
    
    public func start() {
    }

    public func createWindow(title: String, posx: Int32, posY: Int32, width: Int32, height: Int32) -> UnsafeMutableRawPointer? {
        System_CreateWindow(title, posx, posY, width, height)
    }
}