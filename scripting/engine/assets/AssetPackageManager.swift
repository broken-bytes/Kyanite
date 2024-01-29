class AssetPackageManager {
    static let shared = AssetPackageManager()

    private init() {
    }

    lazy var packages: [AssetPackage] = {
        return []
    }()
}