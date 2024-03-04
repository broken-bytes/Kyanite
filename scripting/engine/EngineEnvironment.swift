import WinSDK

class EngineEnvironment {
    static let shared = EngineEnvironment()

    var engineParent: Entity!

    private init() { }

    func configure() {
        // Create the engine parent entity
        engineParent = Entity("World")
    }
}