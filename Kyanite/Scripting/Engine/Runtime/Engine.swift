import Foundation
import Core
import Math
import WinSDK


public func testSystem(iterator: UnsafeMutableRawPointer) {
    OutputDebugStringA("TICK")
}

internal class Engine {
    internal init() {
    }

    internal func start(
        width: UInt32, 
        height: UInt32, 
        window: UnsafeMutableRawPointer, 
        rootDir: UnsafeMutableRawPointer,
        world: String
    ) {
        NativeCore.shared.start(width: width, height: height, window: window, rootDir: rootDir)
        Task {
            try! await WorldManager.shared.loadWorld(name: world)
        }

        let tId = try! NativeCore.shared.registerNewComponent(type: TransformComponent.self, named: "Transform")
        let mId = try! NativeCore.shared.registerNewComponent(type: MeshComponent.self, named: "Mesh")

        NativeCore.shared.registerSystem(callback: testSystem, archetype: [tId, mId])

        let entity = Entity(name: "MainEnt")
        entity.addComponent(component: TransformComponent(
            position: Vector3(x: 0, y: 0, z: 0),
            scale: Vector3(x: 0, y: 0, z: 0),
            rotation: Vector3(x: 0, y: 0, z: 0)))
        entity.addComponent(component: MeshComponent(internalRefId: 0))
    }
    
    internal func update(timestep: Float) {
        NativeCore.shared.update(tick: timestep)
        InputSystem.shared.flush()
    }

    internal func onKeyChanged(key: UInt8, isPressed: Bool, name: String) {
        InputSystem.shared.setKeyboardButton(button: InputSystem.KeyboardButton(rawValue: key)!, isPressed: isPressed, name: name)
    }

    internal func onMouseButtonChanged(button: UInt8, isPressed: Bool) {
        InputSystem.shared.setMouseButton(button: InputSystem.MouseButton(rawValue: button)!, isPressed: isPressed)
    }

    internal func onAxisChanged(axis: UInt8, value: Float) {

    }

    internal func onMousePositionChanged(mouseX: UInt32, mouseY: UInt32) {

    }
}
