// This will be generated at some point, but for now it's manually created
import Foundation
import KyaniteEngine
@_implementationOnly import Native

var isInitialized = false

@_cdecl("kyaniteeditormain")
@available(*, deprecated, message: "Used internally by the Kyanite Editor. Do not call directly.")
public func kyaniteeditormain(at path: String) {
    if isInitialized {
        fatalError("Kyanite Editor has already been initialized")
    }

    // The second argument of the command line arguments is the project path
    isInitialized = true
    let engine = Engine(isDebug: true)
    let editor = Editor()
    editor.start(at: path)
    engine.start()
}
