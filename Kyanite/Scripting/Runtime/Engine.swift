import Core

public class Engine {
    public init() {
        
    }

    public func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, ctx: UnsafeMutableRawPointer, style: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
        NativeCore.shared.start(width: width, height: height, window: window, ctx: ctx, style: style, rootDir: rootDir)
    }
    
    public func update(timestep: Float) {
        NativeCore.shared.update(tick: timestep)
    }
}
