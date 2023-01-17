import Foundation
import Core
import Math



var entities: [Entity] = []

public func testSystem(iterator: UnsafeMutableRawPointer) {
    var transform = NativeCore.shared.getComponentSet(iterator: iterator, type: TransformComponent.self, index: 1)
    var movement = NativeCore.shared.getComponentSet(iterator: iterator, type: MoveComponent.self, index: 2)
    let deltaTime = NativeCore.shared.getSystemDeltaTime(iterator: iterator)
    
    for x in 0..<transform.count {
        transform[x].position = add(left: transform[x].position, right: mul(vector: movement[x].movement, value: deltaTime))
    }

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

        try! ComponentRegistry.shared.register(component: TransformComponent.self)
        try! ComponentRegistry.shared.register(component: MeshComponent.self)
        try! ComponentRegistry.shared.register(component: RigidbodyComponent.self)
        try! ComponentRegistry.shared.register(component: SpriteComponent.self)
        try! ComponentRegistry.shared.register(component: MoveComponent.self)

        NativeCore.shared.registerSystem(name: "RenderingSystem", callback: {it in }, TransformComponent.self, MeshComponent.self)
        NativeCore.shared.registerSystem(name: "PhysicsSystem", callback: {it in }, TransformComponent.self, RigidbodyComponent.self)
        NativeCore.shared.registerSystem(name: "MovementSystem", callback: testSystem, TransformComponent.self, MoveComponent.self)


        for x in 0..<100 {
            let entity = Entity(name: "Entity_\(x)")
            entity.addComponent(component: TransformComponent(
                position: Vector3(x: 1, y: 2, z: 3),
                scale: Vector3(x: 6, y: 5, z: 4),
                rotation: Vector3(x: 0, y: 0, z: 0)))
            entity.addComponent(component: MeshComponent(internalRefId: 0))
            entity.addComponent(component: MoveComponent(movement: Vector3(x: 1, y: 0, z: 0.1)))

            entities.append(entity)
        }
    }
    
    internal func update(timestep: Float) {
        NativeCore.shared.update(tick: timestep)
        InputSystem.shared.flush()
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

    internal func onMousePositionChanged(mouseX: UInt32, mouseY: UInt32) {

    }
}
