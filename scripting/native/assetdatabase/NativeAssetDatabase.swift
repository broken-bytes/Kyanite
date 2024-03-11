@_implementationOnly import Bridge
import WinSDK

public class NativeAssetDatabase {
    public enum NativeAssetDatabaseError: Error {
        case failedToOpenDatabase(Int)
        case failedToAddAsset(Int)
    }
    public static let shared = NativeAssetDatabase()

    private var data: UnsafeMutablePointer<UnsafeMutablePointer<CChar>?>?
    private var count: Int32 = 0

    private init() { }

    public func open(at path: String) -> UnsafeMutableRawPointer {
        AssetDatabase_CreateOrLoad(path)
    }

    public func close(_ database: UnsafeMutableRawPointer) {
        AssetDatabase_Close(database)
    }

    public func addAsset(
        _ database: UnsafeMutableRawPointer, 
        uuid: String, 
        name: String,
        path: String, 
        type: String,
        time: Int64
    ) -> Int32 {
        AssetDatabase_AddAsset(database, uuid, name, path, type, time)
    }

    public func updateAsset(
        _ database: UnsafeMutableRawPointer, 
        uuid: String, 
        time: Int64
    ) {
        AssetDatabase_UpdateAsset(database, uuid, time)
    }

    public func removeAsset(_ database: UnsafeMutableRawPointer, uuid: String) {
        AssetDatabase_RemoveAsset(database, uuid)
    }

    public func getUUID(_ database: UnsafeMutableRawPointer, path: String) -> String {
        var uuidPtr: UnsafePointer<CChar>? = nil
        AssetDatabase_GetUuidForPath(database, path, &uuidPtr)

        guard let uuid = uuidPtr else {
            OutputDebugStringA("UUID for path: \(path) is nil\n")
            return ""
        }

        let string = String(cString: uuid, encoding: .utf8) ?? ""
        // Free the memory allocated by the native code
        AssetDatabase_FreeUuid(uuidPtr)
        return string
    }

    public func getLastModified(_ database: UnsafeMutableRawPointer, uuid: String) -> Int64 {
        AssetDatabase_GetModifiedTime(database, uuid)
    }
}