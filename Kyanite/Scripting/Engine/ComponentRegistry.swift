import Core

internal class ComponentRegistry {

    fileprivate struct ComponentEntry {
        let name: String
        let id: UInt64
    }

    internal static let shared = ComponentRegistry()

    fileprivate var mappings: [String: ComponentEntry] = [:]

    internal init() {

    }

    internal func register<T>(component: T.Type) -> UInt64 {
        let tempName = "\(component)"
        let id = NativeCore.shared.registerNewComponent(type: type(of: component), named: tempName)
        mappings[tempName] = ComponentEntry(name: tempName, id: id)

        return id
    }

    internal func resolve<T>(component: T.Type) -> UInt64 {
        if let entry = mappings["\(component)"] {
            return entry.id
        } else {
            return register(component: component)
        }
    }
 }