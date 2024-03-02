internal class WindowManager {
    internal var windows: [EditorWindow] = []
    
    internal static let shared: WindowManager = WindowManager()

    private init() {

    }
}