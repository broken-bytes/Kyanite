public class Object {
    internal var handle: NativePointer? = nil

    init(handle: NativePointer) {
        self.handle = handle
    }

    deinit {
        if let handle {
            AssetManager.shared.unloadAsset(handle: handle)
        }
    }
}