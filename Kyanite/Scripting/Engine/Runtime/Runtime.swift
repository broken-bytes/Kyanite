#if os(Windows)
import WinSDK
#endif

let engine = Engine()

@_cdecl("start") public func start(
    width: UInt32,
    height: UInt32, 
    window: UnsafeMutableRawPointer, 
    rootDir: UnsafeMutableRawPointer,
    world: UnsafeMutableRawPointer
) {
    let rawStr = world.bindMemory(to: CChar.self, capacity: strlen(world))
    let str = String(cString: rawStr)
    engine.start(width: width, height: height, window: window, rootDir: rootDir, world: str)
}

@_cdecl("update") public func update(tick: Float) {
    engine.update(timestep: tick)
}

@_cdecl("onKeyUp") public func onKeyUpHandler(key: UInt8, name: UnsafeMutableRawPointer) {
    let rawStr = name.bindMemory(to: CChar.self, capacity: strlen(name))
    let str = String(cString: rawStr)
    engine.onKeyChanged(key: key, isPressed: false, name: str)
}

@_cdecl("onKeyDown") public func onKeyDownHandler(key: UInt8, name: UnsafeMutableRawPointer) {
    let rawStr = name.bindMemory(to: CChar.self, capacity: strlen(name))
    let str = String(cString: rawStr)
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

@_cdecl("onMousePositionChanged") public func onMousePositionChanged(mouseX: UInt32, mouseY: UInt32) {
    engine.onMousePositionChanged(mouseX: mouseX, mouseY: mouseY)
}
