import Foundation

class AssetPackageManager {
    static let shared = AssetPackageManager()
    private static let packagePath = "content"

    public let packages: [AssetPackage]
    
    private init() {
        FileManager.default.changeCurrentDirectoryPath(Bundle.main.bundlePath)
        guard var appDir = URL(string: FileManager.default.currentDirectoryPath) else { fatalError("Failed to get app directory") }
        appDir.appendPathComponent(AssetPackageManager.packagePath)

        var numPackages: Int = 0
        let packageHandles = AssetPackages_LoadAssetPackages(appDir.absoluteString.cString(using: .utf8), &numPackages)

        var packages: [AssetPackage] = []
        for x in 0..<numPackages {
            guard let handle = packageHandles?.advanced(by: x) else { fatalError("Failed to load asset package") }
            packages.append(AssetPackage(handle: handle))
        }

        self.packages = packages
    }
}