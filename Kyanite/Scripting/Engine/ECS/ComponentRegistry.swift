import Core

public enum ComponentError: Error {
    case notRegistered(message: String)
}

public class ComponentRegistry {

    fileprivate struct ComponentEntry: Equatable, Hashable {
        let name: String
        let id: UInt64

        static func ==(lhs: ComponentEntry, rhs: ComponentEntry) -> Bool {
            return lhs.id == rhs.id
        }

        func hash(into hasher: inout Hasher) {
            hasher.combine(name)
            hasher.combine(id)
        }
    }

    public static let shared = ComponentRegistry()

    fileprivate var mappings: Set<ComponentEntry> = []

    internal init() {

    }
    
    public func register<T>(component: T.Type) throws -> UInt64 {
        let tempName = "\(component)"
        Logger.shared.println(str: "Registering: \(component) from register")
        let id = try NativeCore.shared.registerNewComponent(type: component)
        mappings.insert(ComponentEntry(name: tempName, id: id))

        return id
    }

    public func resolve<T>(component: T.Type) throws -> UInt64 {
        if let entry = mappings.first(where: { $0.name == "\(component)"}) {
            return entry.id
        } 
        
        throw ComponentError.notRegistered(message: "The component \(component) has not been registerd")
        
    }

    internal func resolveType(type: any Component.Type) throws -> UInt64 {
        if let entry = mappings.first(where: { $0.name == "\(type)"}) {
            return entry.id
        } 
        
        throw ComponentError.notRegistered(message: "The component \(type) has not been registerd")  
    }
 }