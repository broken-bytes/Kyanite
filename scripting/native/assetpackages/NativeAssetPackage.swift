@_implementationOnly import Bridge

public class NativeAssetPackage {
    internal var handle: NativePointer
    
    internal init(handle: NativePointer) {
        self.handle = handle
    }

    public func getShader(_ name: String) -> NativeShader {
        return NativeShader(handle: AssetPackages_LoadShader(handle, name))
    }

    public func getTexture(_ name: String) -> NativeTexture {
        return NativeTexture(handle: AssetPackages_LoadTexture(handle, name))
    }

    public func getMaterial(_ name: String) -> NativeMaterial {
        return NativeMaterial(handle: AssetPackages_LoadMaterial(handle, name))
    }

    public func getMesh(_ name: String) -> NativeMesh {
        return NativeMesh(handle: AssetPackages_LoadMesh(handle, name))
    }
}