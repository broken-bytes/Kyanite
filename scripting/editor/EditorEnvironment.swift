import KyaniteEngine

class EditorEnvironment {
    static let `default` = EditorEnvironment()

    var editorParent: Entity!
    var selectedObject: UInt64? = nil
    internal var projectFolder: String = ""

    internal var buildFolder: String {
        return "\(projectFolder)/.build"
    }
    internal var cacheFolder: String {
        return "\(projectFolder)/.cache"
    }
    internal var assembliesFolder: String {
        return "\(cacheFolder)/assemblies"
    }
    internal var blobsFolder: String {
        return "\(cacheFolder)/blobs"
    }
    internal var metadataFolder: String {
        return "\(cacheFolder)/metadata"
    }
    internal var logsFolder: String {
        return "\(projectFolder)/.logs"
    }
    internal var contentFolder: String {
        return "\(projectFolder)/content"
    }
    internal var srcFolder: String {
        return "\(projectFolder)/src"
    }

    func configure(with projectPath: String) {
        editorParent = Entity(name: "Editor")
        self.projectFolder = projectPath
    }
}