import Foundation
import Core
import Math

#if _EDITOR
import Editor
#endif


struct MoveComponent: Component {
    public var movement: Vector3
}

public func testSystem(iterator: UnsafeMutableRawPointer) {
    let delta = NativeCore.shared.getSystemDeltaTime(iterator: iterator)
    var transforms = NativeCore.shared.getComponentSet(iterator: iterator, type: TransformComponent.self, index: 1)
    var movement = NativeCore.shared.getComponentSet(iterator: iterator, type: MoveComponent.self, index: 2)

    for x in 0..<transforms.count {
        transforms[x].position = add(left: transforms[x].position, right: mul(vector: movement[x].movement, value: delta))
    }
}

internal class Engine {
    #if _EDITOR
    private let editor = Editor()
    #endif
    private var deltaTime: Float = 0
    private var cpuTime: Float = 0
    private var gpuTime: Float = 0    

    internal init() {
    }

    internal func start(
        args: [String],
        width: UInt32, 
        height: UInt32, 
        window: UnsafeMutableRawPointer
    ) {
        args[1].withCString {
            NativeCore.shared.start(width: width, height: height, window: window, rootDir: $0)

        #if _EDITOR
            editor.start(rootDir: args[1])
        #endif
        }

        try! ComponentRegistry.shared.register(component: TransformComponent.self)
        try! ComponentRegistry.shared.register(component: MoveComponent.self)
        try! ComponentRegistry.shared.register(component: MeshComponent.self)
        try! ComponentRegistry.shared.register(component: MaterialComponent.self)
        NativeCore.shared.registerSystem(name: "MovementSystem", callback: testSystem, TransformComponent.self, MoveComponent.self)

        World("Test")

        Entity("Main") {
                TransformComponent(
                    position: Vector3(x: Float.random(in: -100..<101), y: Float.random(in: -100..<101), z: Float.random(in: -100..<101)),
                    scale: Vector3(x: Float.random(in: 0.05..<1.5), y: Float.random(in: 0.05..<1.5), z: Float.random(in: 0.05..<1.5)),
                    rotation: Vector3(x: Float.random(in: -100..<101), y: Float.random(in: -100..<101), z: Float.random(in: -100..<101))
                )
                MoveComponent(
                    movement: Vector3(
                        x: Float.random(in: -1.5..<1.5), 
                        y: 0, 
                        z: Float.random(in: -1.5..<1.5)
                    )
                )
                MeshComponent(mesh: Mesh())
            }
        Entity("Another") {
            TransformComponent(
                position: Vector3(x: Float.random(in: -100..<101), y: Float.random(in: -100..<101), z: Float.random(in: -100..<101)),
                scale: Vector3(x: Float.random(in: 0.05..<1.5), y: Float.random(in: 0.05..<1.5), z: Float.random(in: 0.05..<1.5)),
                rotation: Vector3(x: Float.random(in: -100..<101), y: Float.random(in: -100..<101), z: Float.random(in: -100..<101))
            )
            MoveComponent(
                movement: Vector3(
                    x: Float.random(in: -1.5..<1.5), 
                    y: 0, 
                    z: Float.random(in: -1.5..<1.5)
                )
            )
            MeshComponent(mesh: Mesh())
        }
    }
    
    internal func update() {
        #if _EDITOR
        editor.update()
        #endif
        var start = Date()
        NativeCore.shared.update(tick: deltaTime)
        InputSystem.shared.flush()
        NativeCore.shared.endUpdate()
        var delta: TimeInterval = start.distance(to: Date())
        Timing.shared.deltaTime = Float(delta)
    }

    internal func onKeyChanged(key: UInt8, isPressed: Bool, name: String) {
        guard let keyData = InputSystem.KeyboardButton(rawValue: key) else { return }
        InputSystem.shared.setKeyboardButton(button: keyData, isPressed: isPressed, name: name)
    }

    internal func onMouseButtonChanged(button: UInt8, isPressed: Bool) {
        guard let keyData = InputSystem.MouseButton(rawValue: button) else { return }
        InputSystem.shared.setMouseButton(button: keyData, isPressed: isPressed)
    }

    internal func onAxisChanged(axis: UInt8, value: Float) {

    }

    internal func onMousePositionChanged(mouseX: Int32, mouseY: Int32) {
        InputSystem.shared.setMouseMoved(x: mouseX, y: mouseY)
    }

    internal func onViewportResized(width: UInt32, height: UInt32) {
        NativeCore.shared.setResized(width: width, height: height)
    }

    deinit {
        print("GGOODBYE FROM ENGINE")
    }
}
