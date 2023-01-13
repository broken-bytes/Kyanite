public class Editor {
    private var windows: [EditorWindow] = []

    func start(hwnd: UnsafeMutableRawPointer) {
        windows.append(Outliner())
        windows.append(Profiler())
        windows.append(Universe())
        windows.append(Viewport(hwnd: hwnd))
    }

    func tick(lastTick: Float) {

    }

    func editorTick() {
        for window in windows {
            window.draw()
        }
    }

    func end() {

    }
}