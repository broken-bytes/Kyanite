import Foundation

class Engine {
    var time: Float = 0 

    init() {
    }

    func start() {
        Bridge_Engine_Init(nil, nil, Mode(0), nil)
        let renderSystem = RenderSystem()
        let entity = Entity(name: "Test")
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