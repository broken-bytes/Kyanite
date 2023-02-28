import Foundation

public protocol Component: Hashable {

    static var identifier: Int { get }
    var identifier: Int { get }
    static var size: Int { get }
    var size: Int { get }
    static var stride: Int { get }
    var stride: Int { get }
}

internal extension Component {

    static var identifier: Int { 
        let hash = makeRuntimeHash(Self.self)
        return hash
    }
    var identifier: Int { Self.identifier }
    static var size: Int { MemoryLayout<Self>.size }
    var size: Int { Self.size }
    static var stride: Int { MemoryLayout<Self>.stride }
    var stride: Int { Self.stride }

    static func makeRuntimeHash<C>(_ componentType: C.Type) -> Int where C: Component {
        ObjectIdentifier(componentType).hashValue
    }
}