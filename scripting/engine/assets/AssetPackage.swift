public class AssetPackage {
    public init(path: String) {

    }

    public func hasAsset<T>(type: T.Type, uuid: String) -> Bool {
        return false
    }

    public func getAsset<T>(type: T.Type, uuid: String) -> T? {
        return nil
    }
}