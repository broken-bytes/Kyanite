let engine = Engine()

#if _EDITOR || _RUNTIME
@_cdecl("start") public func start(
    argumentCount: UInt32,
    argumentVector: UnsafeMutablePointer<UnsafeMutablePointer<UInt8>>,
    width: UInt32,
    height: UInt32, 
    window: UnsafeMutableRawPointer
) {
    var args: [String] = []

    for x in 0..<argumentCount {
        let ptr = argumentVector[Int(x)]
        args.append(String(cString: ptr))
        Logger.shared.println(str: String(cString: ptr))
    }

    engine.start(args: args, width: width, height: height, window: window)
}

@_cdecl("update") public func update() {
    engine.update()
}

@_cdecl("onKeyUp") public func onKeyUpHandler(key: UInt8, name: UnsafeMutablePointer<UInt8>) {
    let str = String(cString: name)
    engine.onKeyChanged(key: key, isPressed: false, name: str)
}

@_cdecl("onKeyDown") public func onKeyDownHandler(key: UInt8, name: UnsafeMutablePointer<UInt8>) {
    let str = String(cString: name)
    engine.onKeyChanged(key: key, isPressed: true, name: str)
}

@_cdecl("onMouseButtonUp") public func onMouseButtonUp(button: UInt8) {
    engine.onMouseButtonChanged(button: button, isPressed: true)
}

@_cdecl("onMouseButtonDown") public func onMouseButtonDown(button: UInt8) {
    engine.onMouseButtonChanged(button: button, isPressed: false)
}

@_cdecl("onAxisChanged") public func onAxisChangedHandler(axis: UInt8, value: Float) {
    engine.onAxisChanged(axis: axis, value: value)
}

@_cdecl("onMouseMoved") public func onMousePositionChanged(mouseX: Int32, mouseY: Int32) {
    engine.onMousePositionChanged(mouseX: mouseX, mouseY: mouseY)
}

@_cdecl("onViewportResized") public func onViewportResized(width: UInt32, height: UInt32) {
    engine.onViewportResized(width: width, height: height)
}
#endif

