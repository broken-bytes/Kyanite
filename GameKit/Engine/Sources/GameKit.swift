@_implementationOnly import Core
@_implementationOnly import Rendering
import Foundation

private var mainWindow: Window!

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

        for x in 0..<100000 {
            world.createEntity(named: "Test", Transform(q: 1, w: 3, x: 2, y: 5, z: 4), Move(movement: 1), Rotate(rotation: Float.random(in: -10...10)))
        }


        try! world.createSystem(named: "Rotator", Transform.self, Move.self, Rotate.self, pipeline: ECS.Pipeline.onUpdate) { iter in 
            var transforms = iter.dataFor(Transform.self)
            var rotations = iter.dataFor(Rotate.self)


            for x in 0..<iter.count {
                transforms[x].pointee.x += rotations[x].pointee.rotation
            }

        }

        try! world.createSystem(named: "Mover", Transform.self, Move.self, Move.self, pipeline: ECS.Pipeline.onUpdate) { iter in 
            var transforms = iter.dataFor(Transform.self)
            var movements = iter.dataFor(Move.self)

            for x in 0..<iter.count {
                transforms[x].pointee.x += movements[x].pointee.movement
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

        WindowManager.initialise()
        mainWindow = "MainWindow".withCString {
            return WindowManager.createWindow(
                description: WindowManager.WindowCreationDesc(
                    name: $0, 
                    width: 800, 
                    height: 600, 
                    resizable: false, 
                    fullscreen: false
                )
            )
        }

        Rendering.Renderer.initialise(window: mainWindow, backend: .D3D12, width: 800, height: 600)

        while true {
            WindowManager.handleWindowEvents()
        }
    }
}