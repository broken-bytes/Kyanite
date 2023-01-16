public enum AssetError: Error {
    case notFound(message: String)
    case wrongFormat(message: String)
}

public class AssetDatabase {
    public static let shared = AssetDatabase()

    private init() {

    }

    public func loadAsset<T>(at path: String) throws -> T {
        throw AssetError.notFound(message: "Not implemented")
    }
}