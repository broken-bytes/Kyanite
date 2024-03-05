import Foundation

class ReflectionManager {
    static let shared = ReflectionManager()

    private var reflections: [Reflection] = []
    
    private init() {
        // Load all files in the Reflections folder
        let fileManager = FileManager.default
        let path = URL(fileURLWithPath: "./reflections")
        for file in try! fileManager.contentsOfDirectory(at: path, includingPropertiesForKeys: nil) {
            let data = try! Data(contentsOf: file)
            let reflection = try! JSONDecoder().decode(Reflection.self, from: data)

            self.reflections.append(reflection)
        }
    }
    
    func reflection<T>(for type: T.Type) -> Reflection? {
        return reflections.first { $0.type == String(describing: type) }
    }
}