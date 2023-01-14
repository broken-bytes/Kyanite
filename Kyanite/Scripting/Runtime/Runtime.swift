let engine = Engine()

@_cdecl("start") public func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, ctx: UnsafeMutableRawPointer, style: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
    engine.start(width: width, height: height, window: window, ctx: ctx, style: style, rootDir: rootDir)
}

@_cdecl("update") public func update(tick: Float) {
    engine.update(timestep: tick)
}