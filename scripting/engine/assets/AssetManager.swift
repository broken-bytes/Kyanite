class AssetManager {
    static let shared = AssetManager()

    // Mapping from uuid to native handle
    private var assets: [String: AnyAsset] = [:]

    private init() {
        
    }

    func loadAsset<T>(type: T.Type, uuid: String) -> AnyAsset where T: AssetProtocol {
        // Should load asset only once and cache it

        // Check if asset is already loaded
        if let handle = assets[uuid] {
            return handle
        }

        let packages = AssetPackageManager.shared.packages
        for package in packages {
            if package.hasAsset(uuid: uuid) {
                let handle = package.getAsset(type: type, uuid: uuid)
                assets[uuid] = handle.eraseToAnyAsset()
                
                return handle.eraseToAnyAsset()
            }
        }

        fatalError("Asset with uuid \(uuid) not found")
    }
}