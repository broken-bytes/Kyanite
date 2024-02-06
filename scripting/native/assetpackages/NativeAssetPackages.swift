@_implementationOnly import Bridge

public class NativeAssetPackages {
    public static let shared = NativeAssetPackages()
    
    private init() {}
    
    public func start() {
        // NOOP
    }

    public func loadAssetPackages(_ path: String) -> [NativeAssetPackage] {
        var count: Int = 0
        var rawPtr: UnsafeMutablePointer<NativePointer?>? = nil
        AssetPackages_LoadAssetPackages(path.cString(using: .utf8), rawPtr, &count)

        // Map the buffers to an array of NativeAssetPackage
        let buffer = UnsafeMutableBufferPointer(start: rawPtr, count: count)

        return buffer.compactMap { 
            guard let handle = $0 else { return nil }
            
            return NativeAssetPackage(handle: handle) 
        } 
    }

    public func disposeAsset(native: Object) {
        AssetPackages_DisposeAsset(native.handle)
    }
}