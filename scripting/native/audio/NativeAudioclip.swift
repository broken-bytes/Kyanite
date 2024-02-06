@_implementationOnly import Bridge

public class NativeAudioclip {
    internal var handle: NativePointer
    
    internal init(handle: NativePointer) {
        self.handle = handle
    }
}