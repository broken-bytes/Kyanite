public class AssetPackage {
    internal let handle: UnsafeMutableRawPointer

    public init(handle: UnsafeMutableRawPointer) {
        self.handle = handle
    }

    public func hasAsset(uuid: String) -> Bool {
        return false
    }

    public func getAsset(uuid: String) -> UnsafeMutableRawPointer {
        // Load the asset from the package
        fatalError("Not implemented")
    }
}