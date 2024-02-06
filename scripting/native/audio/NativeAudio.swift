@_implementationOnly import Bridge

public class NativeAudio {
    public static let shared = NativeAudio()

    private init() {}

    public func start() {
        Audio_Init()
    }
}