let editor = Editor()

@_cdecl("start") func start(hwnd: UnsafeMutableRawPointer) {
    editor.start(hwnd: hwnd)
}

@_cdecl("tick") func tick(lastTick: Float) {
    editor.tick(lastTick: lastTick)
}

@_cdecl("editorTick") func editorTick() {
    editor.editorTick()
}