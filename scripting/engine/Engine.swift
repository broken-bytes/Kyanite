import Foundation
import Native

@System class TestSystem {
    public static func run(transform: UnsafeMutableBufferPointer<TransformComponent>) {
        for x in 0..<transform.count {
            transform[x].position.x += InputManager.shared.getKeyState(key: .keycodeA) == .held ? 1 : 0
        }
    }
}

class Engine {
    private var time: Float = 0 
    private var window: NativeWindow? = nil
    var testSystem: TestSystem
    
    init(isDebug: Bool = false) {
        // Initialize all subsystems
        // Initialize the core
        NativeCore.shared.start()
        // Initialize the window
        window = NativeCore.shared.createWindow(
            "Game", 
            posX: nil, 
            posY: nil, 
            width: 800, 
            height: 600, 
            flags: 0, 
            backend: 0, 
            silent: false
        )
        NativeAudio.shared.start()
        NativeInput.shared.start()
        NativeECS.shared.start(debug: isDebug)
        guard let window = window else {
            fatalError("Failed to create window")
        }
        NativeRendering.shared.start(window: window)
        _ComponentRegistry.shared._register(TransformComponent.self)
        testSystem = TestSystem()
    }

    func start() {
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                Renderer.shared.preFrame()
                InputManager.shared.update()
                NativeECS.shared.update(deltaTime: time)
                // Update the rendering system
                Renderer.shared.update(with: time)
                Renderer.shared.postFrame()

                // Debugging: Spawm a new entity when D is pressed
                if InputManager.shared.getKeyState(key: .keycodeD) == .pressed {
                    let entity = Entity(name: UUID().uuidString)
                    entity.addComponent(TransformComponent.self)
                    var transform = TransformComponent()
                    transform.position = Vector3(x: 0, y: 0, z: 0)
                    entity.setComponent(&transform)
                }
            }
            // Attoseconds to milliseconds
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}