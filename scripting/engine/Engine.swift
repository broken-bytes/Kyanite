import Foundation

class Engine {
    var time: Float = 0 

    init() {
    }

    func start() {
        AssetManager.shared.loadAsset(uuid: "0")
        while true {
            var measure = ContinuousClock().measure {
                // Attoseconds to milliseconds
                Bridge_Engine_Update(time)
            }
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}