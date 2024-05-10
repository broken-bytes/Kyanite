enum NewSystemScriptPreset {
    static func make(name: String) -> String {
        // Remove any System suffix, so we always have a clean name and can add it back later
        // We do this because the user might or might not add System to the filename but we always want to have it
        let name = name.replacingOccurrences(of: "System", with: "")

        return """
import Foundation
import Kyanite

@System class \(name)System {
    static func run(transforms: UnsafeMutableBufferPointer<TransformComponent>) {
        // This is run once per frame
        for x in 0..<transforms.count {
            print(transforms[x].position.x)
        }
    }
}
"""
    }
}