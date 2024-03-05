import EditorNative
import KyaniteEngine

@System
class WindowSystem {
    public static func run(windows: UnsafeMutableBufferPointer<WindowComponent>) {
        // First, draw the default imgui dockspace
        EditorNativeCore.shared.drawDefaultImGuiDockspace()
        for window in windows {
            if window.isOpen {
                guard let actualWindow = WindowManager.shared.windows.first(where: { $0.hashValue == window.id }) else {
                    return
                }

                actualWindow.onBeginDraw()
                actualWindow.onDraw()
                actualWindow.onEndDraw()
            }
        }
    }
}