@_implementationOnly import Bridge

public class Object {
    internal var handle: NativePointer? = nil

    init(handle: NativePointer) {
        self.handle = handle
    }

    deinit {
        if let handle {
            AssetPackages_DisposeAsset(handle)
        }
    }
}