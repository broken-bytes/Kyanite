import Foundation
import SQLite

public class AssetDatabase {
    private let rootDir: String
    private var database: UnsafeMutablePointer<OpaquePointer?>!

    public init(rootDir: String) {
        self.rootDir = rootDir
        self.database = nil
        let db = SQLite.sqlite3_open(rootDir.appending("/Cache/assets.db"), database)
    }

    public func asset(for name: String) -> Void {
    }

    public func writeAsset(with name: String, data: Data) {

    }
}