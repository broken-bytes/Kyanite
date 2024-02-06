internal class WindowManager {
    private var windows: [EditorWindow] = []
    
    internal static let shared: WindowManager = WindowManager()

    private init() {
        // Add the inspector window
        windows.append(Inspector())
    }
    
    internal func onDraw() {
        while true {
            for window in windows {
                window.onBeginDraw()
                window.onDraw()
                window.onEndDraw()
            }
        }
    }
}