@_implementationOnly import Bridge
import Foundation

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
    var meshews: [NativeMesh] = []

    init(with: UnsafeMutablePointer<MeshData>, count: Int) {
        for x in 0..<count {
            let mesh = with.advanced(by: x).pointee
            let vertices = Array(UnsafeBufferPointer(start: mesh.vertices, count: Int(mesh.vertexCount)))
            let indices = Array(UnsafeBufferPointer(start: mesh.indices, count: Int(mesh.indexCount)))

            meshews.append(
                NativeMesh(
                    vertices: vertices.map { NativeVertex(position: $0.position, normal: $0.position, uv: $0.texCoord)}, 
                    indices: indices
                )
            )
        }
    }
}

public class NativeAssetPipeline {
    public static let shared = NativeAssetPipeline()

    private init() {}

    public func openZip(at path: String) -> Int8 {
        return 0
    }

    public func loadModel(at path: String) -> NativeMeshList? {
        guard let data = FileManager.default.contents(atPath: path) else {
            return nil
        }

        return withUnsafePointer(to: data) {
            let meshes: UnsafeMutablePointer<MeshData>? = nil
            var count: Int = 0

            AssetPipeline_LoadMeshes($0, data.count, meshes, &count)

            return NativeMeshList(with: meshes!, count: count)
        }
    }
}