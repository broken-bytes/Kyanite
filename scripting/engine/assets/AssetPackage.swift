import Foundation
import Native

public class AssetPackage {

    internal init() {
    }

    public func hasAsset(uuid: String) -> Bool {
        return false
    }

    public func getAsset<T: AssetProtocol>(type: T.Type, uuid: String) -> AssetProtocol {
        precondition(hasAsset(uuid: uuid), "Asset not found")

        fatalError("Asset type not supported")
    }
}