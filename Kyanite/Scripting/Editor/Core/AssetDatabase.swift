import Foundation
import SQLite

public class AssetDatabase {
    private var rootDir: String!
    private var database: UnsafeMutablePointer<OpaquePointer?>!

    public static let shared = AssetDatabase()

    private init() {
        
    }

    public func setRootDir(rootDir: String) {
        self.rootDir = rootDir
        self.database = UnsafeMutablePointer<OpaquePointer?>.allocate(capacity: 1)
        var url = URL(fileURLWithPath: rootDir)
        url.appendPathComponent("Cache")
        url.appendPathComponent("assets.db")
        let db = sqlite3_open(url.absoluteURL.relativePath, database)
        setupDatabase()
    }

    public func asset(for name: String) -> Void {
    }

    /// Links another object to tge given object's prop
    /// - Parameters:
    ///   - object: The object requesting to link
    ///   - prop: The prop to be linked on
    ///   - to: The object that will be linked to prop
    public func setObjectLinkage(object: String, prop: String, linking to: String) {
      
    }

    public func writeAsset(with name: String, path: String, data: Data) {

    }

    private func setupDatabase() {
        setupAssetsTable()
        setupLinksTable()
    }

    private func setupAssetsTable() {
        var error = UnsafeMutablePointer<UnsafeMutablePointer<CChar>?>.allocate(capacity: 512)
          sqlite3_exec(
            database.pointee,
"""
CREATE TABLE IF NOT EXISTS asset (
    uuid CHAR(36) PRIMARY KEY,
    name VARCHAR(255),
    modified TEXT,
    path VARCHAR(255)
);
""",
            nil,
            nil,
            error
        )
        if error.pointee != nil {
            print(String(cString: error.pointee!))
        }
    }

    private func setupLinksTable() {
        var error = UnsafeMutablePointer<UnsafeMutablePointer<CChar>?>.allocate(capacity: 512)
          sqlite3_exec(
            database.pointee,
"""
CREATE TABLE IF NOT EXISTS asset_link (
    id INTEGER PRIMARY KEY,
    "from" CHAR(36),
    "to" CHAR(36),
    field VARCHAR(255)
);
""",
            nil,
            nil,
            error
        )
        if error.pointee != nil {
            print(String(cString: error.pointee!))
        }
    }
}