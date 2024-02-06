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
        // Load the asset from the package
        switch type {
        case is Shader.Type:
            return Shader(uuid: uuid, native: native.getShader(uuid)) as! T
        case is Mesh.Type:
            return Mesh(uuid: uuid, native: native.getMesh(uuid)) as! T
        case is Texture.Type:
            return Texture(uuid: uuid, native: native.getTexture(uuid)) as! T
        default:
            fatalError("Asset type not supported")
        }
    }
}