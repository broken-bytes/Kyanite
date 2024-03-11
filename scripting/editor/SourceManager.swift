class SourceManager {
    private var sourceWatchdog: FilesystemWatchdog

    init(at path: String) {
        let sourcesPath = path.appending("/sources")
        sourceWatchdog = FilesystemWatchdog(at: sourcesPath) { (path, change) in
            print("File \(path) was \(change)")
        }
    }

    func watch() {
        sourceWatchdog.watch()
    }
}