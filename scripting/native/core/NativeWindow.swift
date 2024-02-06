@_implementationOnly import Bridge

public class NativeWindow {
    internal var handle: NativePointer? = nil

    internal init(handle: NativePointer) {
        self.handle = handle
    }
}