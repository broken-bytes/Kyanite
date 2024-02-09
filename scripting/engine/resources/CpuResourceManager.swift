import Foundation

internal class WeakBox<T: AnyObject> {
    weak var value: T?

    init(_ value: T) {
        self.value = value
    }
}

/// The resources manager is responsible for loading and managing resources such as textures, sounds, and fonts.
/// It also keeps track of all the resources that are currently in use.
public class ResourceManager {
    public static let shared = ResourceManager()

    internal var references: [String:AnyObject] = [:]

    private init() {

    }

    public func loadResource<T: AnyObject>(with uuid: String) -> T? {
        // Load the resource from all available asset packages and track it afterwards
        for (_, value) in references {
            if let asset = value as? WeakBox<T> {
                if asset.value != nil {
                    return asset.value!
                } else {
                    references.removeValue(forKey: uuid)
                    return nil
                }
            } else {
                return value as? T
            }
        }

        return nil
    }

    internal func track<T: AnyObject>(_ object: T, retain: Bool = true) {
        if retain {
            references[UUID().uuidString] = object
        } else {
            references[UUID().uuidString] = WeakBox(object)
        }
    }
}