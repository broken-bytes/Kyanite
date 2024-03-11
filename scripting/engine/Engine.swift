import Foundation
@_implementationOnly import Native

public class Engine {
    var time: Float = 0
    
    public init(isDebug: Bool = false) {
        // Initialize all subsystems
        // Initialize the core
        var imGui = NativeImGui.shared.createContext()
        NativeImGui.shared.setContext(imGui)
        // Initialize the window
        NativeAudio.shared.start()
        NativeInput.shared.start(imGui: imGui)
        NativeECS.shared.start(debug: isDebug)
        NativeSystem.shared.start()
        guard let window = NativeSystem.shared.createWindow(
            title: "Kyanite Editor", 
            posx: 100, 
            posY: 100, 
            width: 1920, 
            height: 1080
        ) else {
            print("Failed to create window")
            fatalError("Failed to create window")
        }
        NativeRendering.shared.start(window: window, imGui: imGui)
        _ComponentRegistry.shared._register(TransformComponent.self)
    }

    public func start() {
        EngineEnvironment.shared.configure()
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                Renderer.shared.preFrame()
                Input.shared.update()
                NativeECS.shared.update(deltaTime: time)
                // Update the rendering system
                Renderer.shared.update(with: time)
                Renderer.shared.postFrame()

                Input.shared.when(.keycodeW, is: .pressed) {
                    let entity = Entity(name: "Entity - \(UUID().uuidString)")
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