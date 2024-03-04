import KyaniteEngine
import EditorNative
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
        let imGuiContext = NativeCore.shared.getImGuiContext()
        EditorNativeCore.shared.start(imGuiContext: imGuiContext)
        _ComponentRegistry.shared._register(WindowComponent.self)
        windowSystem =  WindowSystem()
        WindowManager.shared.windows.append(Hierarchy())
        WindowManager.shared.windows.append(Inspector())
    }
}