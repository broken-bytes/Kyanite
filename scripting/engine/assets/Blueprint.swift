import Foundation
import Yams

public class Blueprint: Codable {
    internal func instantiate() -> Entity {
        fatalError("Not implemented")
    }

    internal func store(to file: String) {
        let data = try? YAMLEncoder().encode(self)
        FileManager.default.createFile(atPath: file, contents: data?.data(using: .utf8))
    }

    static internal func load<T>(from file: String) -> T where T : Blueprint {
        fatalError("Not implemented")
    }
}