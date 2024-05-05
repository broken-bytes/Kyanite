import KyaniteEngine
@_implementationOnly import Native

class Editor {
    var windowSystem: WindowSystem?
    var contentManager: ContentManager?
    var sourceManager: SourceManager?
    
    init() {
    }

    func start(at projectPath: String) {
        EditorEnvironment.default.configure(with: projectPath)
        let container = EditorContainer(with: EditorEnvironment.default)
        contentManager = ContentManager(at: projectPath, with: container.getAssetDatabase())
        sourceManager = SourceManager(at: projectPath)
        // Create the editor environment
        // Create the editor parent entity
        let imGuiContext = NativeImGui.shared.getContext()
        _ComponentRegistry.shared._register(WindowComponent.self)
        windowSystem =  WindowSystem()

        guard let contentManager else {
            fatalError("Content Manager is nil")
        }
        guard let sourceManager else {
            fatalError("Source Manager is nil")
        }

        print("Starting the editor")

        WindowManager.shared.windows.append(Hierarchy())
        WindowManager.shared.windows.append(Inspector())
        WindowManager.shared.windows.append(ContentBrowser(contentManager: contentManager))
        WindowManager.shared.windows.append(Terminal())

        contentManager.watch()
        sourceManager.watch()

        // Load the scene
        SceneManager.default.loadScene(scene: TestScene())
    }
}