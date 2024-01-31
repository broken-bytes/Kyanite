import Foundation

class Engine {
    var time: Float = 0 

    init() {
    }

    func start() {
        Bridge_Engine_Init(nil, nil, Mode(0), nil)
        _ComponentRegistry.shared._register(TransformComponent.self)
        _ComponentRegistry.shared._register(MeshComponent.self)
        let renderSystem = RenderSystem()
        let entity = Entity(name: "Test")
        entity.addComponent(TransformComponent.self)
        entity.addComponent(MeshComponent.self)
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                InputManager.shared.update()
                Bridge_Engine_Update(time)
                // Update the swift internal systems
            }
            // Attoseconds to milliseconds
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}