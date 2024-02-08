import Foundation

internal class WeakBox<T: AnyObject> {
    weak var value: T?

    init(_ value: T) {
        self.value = value
    }
}

/// The resources manager is responsible for loading and managing resources such as textures, sounds, and fonts.
public class ResourceManager {
    public static let shared = ResourceManager()

    internal var references: [String:AnyObject] = [:]

    private init() {
        Thread.detachNewThread {
            while true {
                // Occasionally clean up all dead weak references
                Thread.sleep(forTimeInterval: 0.1)
                self.references = self.references.filter {
                    if let value = $0.value as? WeakBox<AnyObject> {
                        return value.value != nil
                    }
                    return true
                }
            }
        }
    }

    internal func track<T: AnyObject>(_ object: T, retain: Bool = true) {
        if retain {
            references[UUID().uuidString] = object
        } else {
            references[UUID().uuidString] = WeakBox(object)
        }
    }
}