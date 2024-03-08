import KyaniteEngine
import Native
import WinSDK

class Editor {
    var windowSystem: WindowSystem?
    
    init() {
    }

    func start() {
        // Create the editor environment
        EditorEnvironment.shared.configure()
        // Create the editor parent entity
        let imGuiContext = NativeImGui.shared.getContext()
        _ComponentRegistry.shared._register(WindowComponent.self)
        windowSystem =  WindowSystem()
        WindowManager.shared.windows.append(Hierarchy())
        WindowManager.shared.windows.append(Inspector())
    }
}