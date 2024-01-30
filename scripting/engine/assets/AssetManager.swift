class AssetManager {
    static let shared = AssetManager()

    // Mapping from uuid to native handle
    private var assets: [String: UnsafeMutableRawPointer] = [:]

    private init() {
        
    }

    func loadAsset(uuid: String) -> UnsafeMutableRawPointer {
        // Should load asset only once and cache it

        // Check if asset is already loaded
        if let handle = assets[uuid] {
            return handle
        }

        let packages = AssetPackageManager.shared.packages
        for package in packages {
            if package.hasAsset(uuid: uuid) {
                let handle: UnsafeMutableRawPointer = package.getAsset(uuid: uuid)
                assets[uuid] = handle
                return handle
            }
        }

        fatalError("Asset with uuid \(uuid) not found")
    }
}