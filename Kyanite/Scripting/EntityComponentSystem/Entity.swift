import Foundation

public class Entity {
    public var name: String
    public let uuid: String
    public internal(set) var components: [Component]
    public internal(set) var componentPtrs: [UnsafeMutablePointer<Component>] = []

    public init(
      name: String,
      components: [Component]) {
        self.name = name
        self.components = components
        self.uuid = UUID().uuidString
    }

    public func componentOf_<T>(type: T.Type) -> UnsafeMutablePointer<T>? {
      for x in 0..<components.count {
            if let _ = components[x] as? T {
                return withUnsafeMutablePointer(to: &components[x]) { ptr in 
                    return UnsafeMutableRawPointer(ptr).assumingMemoryBound(to: T.self)
                }
            }
        }
        return nil
    }

    public func componentOf<T>(type: T.Type) -> UnsafeMutablePointer<T>? {
      for x in 0..<componentPtrs.count {
          let ptr = componentPtrs[x].pointee
            if let _ = componentPtrs[x].pointee as? T {
                return UnsafeMutableRawPointer(componentPtrs[x])?.assumingMemoryBound(to: T.self)
            }
        }
        return nil
    }

    public func attach(_ component: Component) {
      let ptr = UnsafeMutablePointer<Component>.allocate(capacity: 1)
      ptr.initialize(to: component)
      self.componentPtrs.append(ptr)
    }
}
