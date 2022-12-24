let editor = Editor()

@_cdecl("start") func start(hwnd: UnsafeMutableRawPointer) {
    editor.start()
}

@_cdecl("tick") func tick() {
    editor.tick()
}

@_cdecl("editorTick") func editorTick() {
    editor.editorTick()
}