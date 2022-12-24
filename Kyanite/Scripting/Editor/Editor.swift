public class Editor {
    private var windows: [EditorWindow] = []

    func start() {
        windows.append(Outliner())
        windows.append(Profiler())
        windows.append(Universe())
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