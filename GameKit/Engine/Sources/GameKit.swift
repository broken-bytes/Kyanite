@_implementationOnly import Rendering
import Foundation

private class MainThread: Thread {
    var running: Bool = true

    override func main() {
        print("Starting MainThread")
        
        struct Transform {
            var q: Float
            var w: Float
            var x: Float
            var y: Float
            var z: Float
        }

        struct Move {
            var movement: Float
        }

        struct Rotate {
            var rotation: Float
        }

        struct Player {
            let id: Int
        }

        struct Enemy {
            let id: Int
        }

        struct Health {
            let health: Float
        }

        var world = ECS.createWorld()

        print("Transform: \(world.registerComponent(Transform.self))")
        print("Move: \(world.registerComponent(Move.self))")
        print("Rotate: \(world.registerComponent(Rotate.self))")
        print("Player: \(world.registerComponent(Player.self))")
        print("Enemy: \(world.registerComponent(Enemy.self))")
        print("Health: \(world.registerComponent(Health.self))")

        for x in 0..<5000 {
            if x % 2 == 0 {
                world.createEntity(named: "Test", Transform(q: 1, w: 3, x: 2, y: 5, z: 4), Move(movement: 1), Rotate(rotation: 0))
            } else if x % 4 == 0 {
                world.createEntity(named: "Test", Transform(q: 1, w: 3, x: 2, y: 5, z: 4), Move(movement: 1))
            } else {
                world.createEntity(named: "Test", Move(movement: 1), Transform(q: 1, w: 3, x: 2, y: 5, z: 4))
            }
        }


        try! world.createSystem(named: "Rotator", Transform.self, Rotate.self, pipeline: ECS.Pipeline.onUpdate, multithreaded: true) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Rotate.self, at: x).pointee.rotation
            }
        }
        
        try! world.createSystem(named: "Rotator2", Transform.self, Rotate.self, pipeline: ECS.Pipeline.onUpdate) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Rotate.self, at: x).pointee.rotation
            }
        }

        try! world.createSystem(named: "Mover", Transform.self, Move.self, pipeline: ECS.Pipeline.onUpdate) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Move.self, at: x).pointee.movement
            }
        }
        
        try! world.createSystem(named: "Mover2", Transform.self, Move.self, pipeline: ECS.Pipeline.onUpdate, multithreaded: true) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Move.self, at: x).pointee.movement
            }
        }
        
        try! world.createSystem(named: "Move3", Transform.self, Move.self, pipeline: ECS.Pipeline.onUpdate) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Move.self, at: x).pointee.movement
            }
        }
        
        try! world.createSystem(named: "Mover4", Transform.self, Move.self, pipeline: ECS.Pipeline.onUpdate) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Move.self, at: x).pointee.movement
            }
        }
        
        try! world.createSystem(named: "MoveRotator", Transform.self, Move.self, Rotate.self, pipeline: ECS.Pipeline.onUpdate) { iter in
            for x in 0..<iter.count {
                iter.dataFor(Transform.self, at: x).pointee.x += iter.dataFor(Move.self, at: x).pointee.movement
            }
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
