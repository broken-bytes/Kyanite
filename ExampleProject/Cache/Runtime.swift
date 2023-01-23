import Userland
import Engine
import Foundation
import Math


// Pointer to the engine, Ptr to component register func

@_cdecl("start") public func start(
    engine: UnsafeMutableRawPointer
) {
    let engine = engine.load(as: Engine.self)
    try! ComponentRegistry.shared.register(component: TransformComponent.self)
    try! ComponentRegistry.shared.register(component: MeshComponent.self)
    try! ComponentRegistry.shared.register(component: MaterialComponent.self)
    try! ComponentRegistry.shared.register(component: MoveComponent.self)

    World("Test")

    Entity("Test") { 
        TransformComponent(
            position: Vector3(x: 0, y: 0, z: 0),
            scale: Vector3(x: 0, y: 0, z: 0),
            rotation: Vector3(x: 0, y: 0, z: 0)
        )
        MoveComponent(
            movement: Vector3(x: 0, y: 0, z: 0)
        )
    }

    System("MoveSystem") { 
        let delta: Float = $0
        let trans: UnsafeMutablePointer<TransformComponent> = $1
        let move: UnsafeMutablePointer<MoveComponent> = $2
    }
}