@_implementationOnly import Bridge
import Foundation

public enum NativeAssetPipelineError: Error {
    case internalLibraryError(String)
    case emptyResult
}

public struct NativeVertex {
    public var position: (Float, Float, Float)
    public var normal: (Float, Float, Float)
    public var uv: (Float, Float)
}

public struct NativeMesh {
    public var vertices: [NativeVertex]
    public var indices: [UInt32]
}

public class NativeMeshList {
    public var meshes: [NativeMesh] = []

    init(with: UnsafeMutablePointer<MeshData>, count: Int) {
        for x in 0..<count {
            let mesh = with.advanced(by: x).pointee
            let vertices = Array(UnsafeBufferPointer(start: mesh.vertices, count: Int(mesh.vertexCount)))
            let indices = Array(UnsafeBufferPointer(start: mesh.indices, count: Int(mesh.indexCount)))

            meshes.append(
                NativeMesh(
                    vertices: vertices.map { NativeVertex(position: $0.position, normal: $0.position, uv: $0.texCoord)},
                    indices: indices
                )
            )
        }
    }
}

// TODO: Load the whole model instead of just the meshes.
// See: core/editor/assetpipeline/include/loaders/FbxLoader.hxx
public class NativeAssetPipeline {
    public static let shared = NativeAssetPipeline()

    private init() {}

    public func openZip(at path: String) -> Int8 {
        return 0
    }

    public func loadModel(at path: String) -> Swift.Result<NativeMeshList, NativeAssetPipelineError> {
        var meshData: UnsafeMutablePointer<MeshData>? = nil
        var len: Int = -1

        let result = AssetPipeline_LoadMeshes(path, &meshData, &len)

        guard len > 0 else {
            return .failure(.emptyResult)
        }

        guard result.error == nil else {
            return .failure(.internalLibraryError(String(cString: result.error!)))
        }

        return .success(NativeMeshList(with: meshData!, count: len))
    }
}