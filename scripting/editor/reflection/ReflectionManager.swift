import Foundation
import WinSDK

class ReflectionManager {
    static let shared = ReflectionManager()

    private var reflections: [Reflection] = []
    
    private init() {
        // Load all files in the Reflections folder
        let fileManager = FileManager.default
        let path = fileManager.currentDirectoryPath.appendingPathComponent("reflection")
        OutputDebugStringA("Loading reflections from \(path)\n")
        
        do {
            let files = try fileManager.contentsOfDirectory(atPath: path)
            // For each file, load the reflection from json
            for file in files {
                let url = URL(fileURLWithPath: path.appendingPathComponent(file))
                guard let data = FileManager.default.contents(atPath: url.path) else {
                    OutputDebugStringA("Failed to load reflection from \(url)\n")
                    continue
                }
                OutputDebugStringA("Data \(String(data: data, encoding: .utf8) ?? "")\n")
                let reflection = try JSONDecoder().decode(Reflection.self, from: data)
                reflections.append(reflection)
            }
        } catch {
            OutputDebugStringA("Failed to load reflections: \(error)\n")
        }
    }
    
    func reflection(for type: Any.Type) -> Reflection? {
        reflections.first { $0.type == String(describing: type) }
    }
}
