// This will be generated at some point, but for now it's manually created
import Foundation
import KyaniteEngine

@_cdecl("kyaniteeditormain")
public func kyaniteeditormain(window: UnsafeMutableRawPointer?) {
    let engine = Engine(window: window, isDebug: true)
    
    let editor = Editor()
    editor.start()
    engine.start()
}
