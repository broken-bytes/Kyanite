class EditorContainer {    
    private var assetDatabase: AssetDatabase

    init(with environment: EditorEnvironment) {
        assetDatabase = AssetDatabase(path: environment.cacheFolder + "/assetdb.db")
    }

    func getAssetDatabase() -> AssetDatabase {
        assetDatabase
    }
}