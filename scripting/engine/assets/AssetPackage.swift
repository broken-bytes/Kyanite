import Native

public class AssetPackage {
    internal let native: NativeAssetPackage

    internal init(native: NativeAssetPackage) {
        self.native = native
    }

    public func hasAsset(uuid: String) -> Bool {
        return false
    }

    public func getAsset<T: AssetProtocol>(type: T.Type, uuid: String) -> T {
        fatalError("Asset type not supported")
    }
}