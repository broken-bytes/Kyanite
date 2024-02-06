import Foundation
import Native

public class EditorWindow {
    public var title: String { "Editor" }
    private var open: Bool = false

    internal init() {
        open = true
    }


    public func onDraw() {

        // Draw the editor window
    }

    internal func onBeginDraw() {
        NativeRendering.shared.startWindow(title, open: &open)
    }
    
    internal func onEndDraw() {
        NativeRendering.shared.endWindow()        
    }
}