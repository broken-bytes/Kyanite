import Core

internal class Engine {
    internal init() {
    }

    internal func start(width: UInt32, height: UInt32, window: UnsafeMutableRawPointer, ctx: UnsafeMutableRawPointer, style: UnsafeMutableRawPointer, rootDir: UnsafeMutableRawPointer) {
        NativeCore.shared.start(width: width, height: height, window: window, ctx: ctx, style: style, rootDir: rootDir)
        let entity = Entity(name: "Test")
        entity.addComponent(component: ColliderComponent())
    }
    
    internal func update(timestep: Float) {
        NativeCore.shared.update(tick: timestep)
    }
}
