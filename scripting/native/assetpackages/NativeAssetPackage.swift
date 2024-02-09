@_implementationOnly import Bridge

public class NativeAssetPackage {
    internal var handle: NativePointer
    
    internal init(handle: NativePointer) {
        self.handle = handle
    }

    public func getShader(_ uuid: String) -> NativeShader {
        return NativeShader(handle: AssetPackages_LoadShader(handle, uuid))
    }

    public func getTexture(_ uuid: String) -> NativeTexture {
        return NativeTexture(handle: AssetPackages_LoadTexture(handle, uuid))
    }

    public func getMaterial(_ uuid: String) -> NativeMaterial {
        return NativeMaterial(handle: AssetPackages_LoadMaterial(handle, uuid))
    }

    public func getModel(_ uuid: String) -> NativeModel {
        var ids: UnsafeMutablePointer<UInt8>? = nil
        var count: Int = 0

        AssetPackages_LoadModel(handle, uuid.cString(using: .utf8), &ids, &count)

        var idsArray = [UInt8]()
        let buffPtr = UnsafeBufferPointer(start: ids, count: count)
        for id in buffPtr {
            idsArray.append(id)
        }

        return NativeModel(uuid: uuid, meshIds: idsArray)
    }

    public func getMesh(
        _ uuid: String, 
        id: UInt8
    ) -> NativeMesh {
        var vertices: UnsafeMutablePointer<Float>? = nil
        var indices: UnsafeMutablePointer<UInt32>? = nil
        var verticesCount: Int = 0
        var indicesCount: Int = 0

        AssetPackages_LoadMesh(
            handle, 
            uuid.cString(using: .utf8), 
            id, 
            &vertices, 
            &verticesCount, 
            &indices,
            &indicesCount
        )

        return NativeMesh(
            uuid: uuid, 
            vertices: UnsafeMutableBufferPointer(start: vertices, count: verticesCount), 
            indices: UnsafeMutableBufferPointer(start: indices, count: indicesCount)
        )
    }
}