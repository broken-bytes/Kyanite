public class AssetPackage {
    internal let handle: NativePointer

    public init(handle: NativePointer) {
        self.handle = handle
    }

    public func hasAsset(uuid: String) -> Bool {
        return false
    }

    public func getAsset<T>(type: T.Type, uuid: String) -> NativePointer {
        // Load the asset from the package
        switch type {
        case is Shader.Type:
            return AssetPackages_LoadShader(handle, uuid)
        case is Mesh.Type:
            return AssetPackages_LoadMesh(handle, uuid)
        case is Texture.Type:
            return AssetPackages_LoadTexture(handle, uuid)
        default:
            fatalError("Asset type not supported")
        }
    }
}