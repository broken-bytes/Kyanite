import Foundation
import Native

class AssetPackageManager {
    static let shared = AssetPackageManager()
    private static let packagePath = "content"

    public let packages: [AssetPackage]
    
    private init() {
        FileManager.default.changeCurrentDirectoryPath(Bundle.main.bundlePath)
        guard var appDir = URL(string: FileManager.default.currentDirectoryPath) else { fatalError("Failed to get app directory") }
        appDir.appendPathComponent(AssetPackageManager.packagePath)

        self.packages = NativeAssetPackages.shared.loadAssetPackages(appDir.absoluteString).map { AssetPackage(native: $0) }
    }
}