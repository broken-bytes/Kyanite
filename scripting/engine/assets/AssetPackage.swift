import Foundation
import Native

public class AssetPackage {
    internal let native: NativeAssetPackage

    internal init(native: NativeAssetPackage) {
        self.native = native
    }

    public func hasAsset(uuid: String) -> Bool {
        return false
    }

    public func getAsset<T: AssetProtocol>(type: T.Type, uuid: String) -> AssetProtocol {
        precondition(hasAsset(uuid: uuid), "Asset not found")

        switch type {
            case is Model.Type:
                return Model(uuid: uuid, native: native.getModel(uuid))
            default:
                fatalError("Asset type not supported")
        }
        fatalError("Asset type not supported")
    }
}