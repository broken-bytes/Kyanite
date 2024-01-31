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
            return Bridge_Engine_LoadShader(handle, uuid)
        case is Mesh.Type:
            return Bridge_Engine_LoadMesh(handle, uuid)
        case is Texture.Type:
            return Bridge_Engine_LoadTexture(handle, uuid)
        default:
            fatalError("Asset type not supported")
        }
    }
}