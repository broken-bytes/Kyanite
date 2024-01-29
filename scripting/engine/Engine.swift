import Foundation

class Engine {
    var time: Float = 0 

    init() {
    }

    func start() {
        let material = MaterialComponent()
        print(material)
        let encoder = JSONEncoder()
        encoder.outputFormatting = .prettyPrinted
        let data = try! encoder.encode(material)
        print(String(data: data, encoding: .utf8)!)

        while true {
            var measure = ContinuousClock().measure {
                // Attoseconds to milliseconds
                Bridge_Engine_Update(time)
            }
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}