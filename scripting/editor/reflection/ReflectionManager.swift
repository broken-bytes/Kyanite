import Foundation

class ReflectionManager {
    static let shared = ReflectionManager()

    private var reflections: [Reflection] = []
    
    private init() {
        // Load all files in the Reflections folder
        let fileManager = FileManager.default
        let path = fileManager.currentDirectoryPath.appendingPathComponent("reflection")
        
        do {
            let files = try fileManager.contentsOfDirectory(atPath: path)
            // For each file, load the reflection from json
            for file in files {
                let url = URL(fileURLWithPath: path.appendingPathComponent(file))
                guard let data = FileManager.default.contents(atPath: url.path) else {
                    continue
                }
                let reflection = try JSONDecoder().decode(Reflection.self, from: data)
                reflections.append(reflection)
            }
        } catch {
        }
    }
    
    func reflection(for type: Any.Type) -> Reflection? {
        reflections.first { $0.type == String(describing: type) }
    }
}
