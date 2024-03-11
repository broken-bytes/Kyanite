@_implementationOnly import Native
import WinSDK

public class AssetDatabase {    
    private var database: UnsafeMutableRawPointer?

    public init(path: String) { 
        database = NativeAssetDatabase.shared.open(at: path)
    }
    
    public func close() {
        NativeAssetDatabase.shared.close(database!)
    }

    public func addAsset(uuid: String, name: String, path: String, type: String, time: Int64)  {
        OutputDebugStringA("Adding asset: uuid: \(uuid) \(name) path: \(path) time: \(time)\n")
        
        let result = NativeAssetDatabase.shared.addAsset(database!, uuid: uuid, name: name, path: path, type: type, time: time) 
        if result != 0 {
            OutputDebugStringA("Failed to add asset: uuid: \(uuid) \(name) path: \(path) time: \(time) result: \(result)\n")
        }
    }

    public func updateAsset(uuid: String, time: Int64) {
        NativeAssetDatabase.shared.updateAsset(database!, uuid: uuid, time: time)
    }

    public func removeAsset(uuid: String) {
        NativeAssetDatabase.shared.removeAsset(database!, uuid: uuid)
    }

    public func getUUID(path: String) -> String {
        NativeAssetDatabase.shared.getUUID(database!, path: path)
    }

    public func getLastModified(uuid: String) -> Int64 {
        NativeAssetDatabase.shared.getLastModified(database!, uuid: uuid)
    }

    deinit {
        close()
    }
}