// This will be generated at some point, but for now it's manually created
import Foundation
import KyaniteEngine

@_cdecl("kyaniteeditormain")
public func kyaniteeditormain() {
    let engineThread = Thread {
        let engine = Engine(isDebug: true)
        engine.start()
    }.start()

    let editor = Editor()
    editor.start()
}
