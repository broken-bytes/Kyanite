import KyaniteEngine
import EditorNative
import Native
import WinSDK

class Editor {
    var windowSystem: WindowSystem?
    init() {
    }

    func start() {
        let imGuiContext = NativeCore.shared.getImGuiContext()
        OutputDebugStringA("Starting Editor with context \(imGuiContext)")
        EditorNativeCore.shared.start(imGuiContext: imGuiContext)
        OutputDebugStringA("Started Editor")
        _ComponentRegistry.shared._register(WindowComponent.self)
        OutputDebugStringA("Registered WindowComponent")
        windowSystem =  WindowSystem()
        OutputDebugStringA("Created WindowSystem")
        WindowManager.shared.windows.append(Hierarchy())
        WindowManager.shared.windows.append(Inspector())
    }
}