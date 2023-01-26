import Core

internal typealias LoadAsset        = @convention(c) (UnsafePointer<Int8>) -> UInt64
internal typealias LoadAssets       = @convention(c) (UnsafePointer<Int8>, UnsafeMutableRawPointer) -> Void
internal typealias CreateMaterial   = @convention(c) (UnsafePointer<Int8>, UInt64) -> UInt64

internal struct ModelIdInfo {
    let ids: UnsafeMutablePointer<UInt64>
    let idCount: UInt64
}

public enum AssetError: Error {
    case notFound(message: String)
    case wrongFormat(message: String)
}

public class AssetDatabase {
    private let loadShader: LoadAsset
    private let loadMeshes: LoadAssets
    private let loadTexture: LoadAsset
    private let createMaterial: CreateMaterial

    public static let shared = AssetDatabase()

    private init() {
        let lib = Core.Runtime.library
        self.loadShader = lib.loadFunc(named: "Engine_LoadShader")
        self.loadMeshes = lib.loadFunc(named: "Engine_LoadMeshes")
        self.loadTexture = lib.loadFunc(named: "Engine_LoadTexture")
        self.createMaterial = lib.loadFunc(named: "Engine_LoadMaterialGPU")
    }

    public func loadAsset<T>(at path: String) throws -> T {
        throw AssetError.notFound(message: "Not implemented")
    }

    public func loadShader(at path: String) -> UInt64 {
        return path.withCString {
            return self.loadShader($0)
        }
    }

    public func loadModel(at path: String) -> [UInt64] {
        var rawPtr = UnsafeMutablePointer<ModelIdInfo>.allocate(capacity: 1)
        return path.withCString { cStr in 
            var raw: [UInt64] = []

            self.loadMeshes(cStr, rawPtr)

            return UnsafeMutableBufferPointer<UInt64>(start: rawPtr.pointee.ids, count: Int( rawPtr.pointee.idCount)).map { $0 }
        }
    }

    public func loadTexture(at path: String) -> UInt64 {
        return path.withCString {
            return self.loadTexture($0)
        }
    }

    public func createNewMaterial(named: String, shaderId: UInt64) -> UInt64 {
        return named.withCString {
            return self.createMaterial($0, shaderId)
        }
    }
}