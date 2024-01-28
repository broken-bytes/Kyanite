public class Window {
    internal var ptr: UnsafeMutableRawPointer?
    
    public init(title: String) {
        // TODO: Implement this
    }

    open func onDraw() {
        
    }

    internal func onShow(with window: UnsafeMutableRawPointer?) {
        self.ptr = window
    }
}