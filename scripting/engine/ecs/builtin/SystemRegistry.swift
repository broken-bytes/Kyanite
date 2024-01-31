public class _SystemRegistry {
    static let shared = _SystemRegistry()

    private init() {}

    internal var systems: [Int: UInt64] = [:]

    public func _register<A>(runFunc: @escaping (A) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 
            var components: NativePointer? = nil
            var numComponents: Int = 0
            Bridge_Engine_GetComponentsFromIterator(iter, 0, &components, &numComponents)
        }
    }

    public func _register<A, B>(runFunc: @escaping (A, B) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 

        }
    }

    public func _register<A, B, C>(runFunc: @escaping (A, B, C) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 

        }
    }

    public func _register<A, B, C, D>(runFunc: @escaping (A, B, C, D) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 

        }
    }

    public func _register<A, B, C, D, E>(runFunc: @escaping (A, B, C, D, E) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 

        }
    }

    public func _register<A, B, C, D, E, F>(runFunc: @escaping (A, B, C, D, E, F) -> Void) {
        var id = Bridge_Engine_RegisterSystem { iter in 

        }
    }
}