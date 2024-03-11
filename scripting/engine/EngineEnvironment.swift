@_implementationOnly import Native

class EngineEnvironment {
    static let shared = EngineEnvironment()

    var _engineParent: Entity? = nil

    var engineParent: Entity {
        get {
            if _engineParent == nil {
                _engineParent = Entity.find(by: "World")
            }
            
            return _engineParent!
        }
        set {
            _engineParent = newValue
        }
    }

    private init() { }

    func configure() {
        // Create the engine parent entity
        engineParent = Entity("World")
    }
}