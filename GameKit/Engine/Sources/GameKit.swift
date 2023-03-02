@_implementationOnly import Rendering
import Foundation

private class MainThread: Thread {
    var running: Bool = true

    override func main() {
        print("Starting MainThread")
        
        struct Transform: Codable {
            var q: Float
            var w: Float
            var x: Float
            var y: Float
            var z: Float
        }

        struct Move: Codable {
            var movement: Float
        }

        struct Rotate: Codable {
            var rotation: Float
        }

        struct Player: Codable {
            let id: Int
        }

        struct Enemy: Codable {
            let id: Int
        }

        struct Health: Codable {
            let health: Float
        }

        var world = ECS.createWorld()

        print("Transform: \(world.registerComponent(Transform.self))")
        print("Move: \(world.registerComponent(Move.self))")
        print("Rotate: \(world.registerComponent(Rotate.self))")
        print("Player: \(world.registerComponent(Player.self))")
        print("Enemy: \(world.registerComponent(Enemy.self))")
        print("Health: \(world.registerComponent(Health.self))")

        for x in 0..<50000 {
            if x % 2 == 0 {
                world.createEntity(named: "Test", Transform(q: 1, w: 3, x: 2, y: 5, z: 4), Move(movement: 1), Rotate(rotation: 0))
            } else if x % 4 == 0 {
                world.createEntity(named: "Test", Transform(q: 1, w: 3, x: 2, y: 5, z: 4), Move(movement: 1))
            } else {
                world.createEntity(named: "Test", Move(movement: 1), Transform(q: 1, w: 3, x: 2, y: 5, z: 4))
            }
        }
        
        try! world.createSystem(named: "Rotator", pipeline: ECS.Pipeline.onUpdate) { (data: [(UnsafeMutablePointer<Transform>, UnsafeMutablePointer<Rotate>)]) -> Void in
            data[0].0.pointee.x += data[0].1.pointee.rotation
        }
        
        while running {
            world.tick()
        }
    }
}

public final class GameKit {
    private var mainThread: MainThread

    public init() {
        mainThread = MainThread()
        mainThread.start()

        //Rendering.Renderer.initialise(window: mainWindow, backend: .D3D12, width: 800, height: 600)

        while true {
        }
    }
}
