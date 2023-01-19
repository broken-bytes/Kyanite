public class Editor {
    private var windows: [EditorWindow] = []

    public init() {

    }

    public func start() {
        windows.append(Outliner())
        windows.append(Profiler())
        windows.append(Universe())
    }

    public func update() {

    }

    public func editorUpdate() {
        for window in windows {
            window.draw()
        }
    }

    public func end() {

    }
}