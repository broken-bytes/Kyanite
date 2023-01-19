public protocol SwiftSymbol {
    var name: String { get }
}

public struct Class: SwiftSymbol {
    public let name: String
    public let inheritedTypes: [Class]
}

public struct Struct: SwiftSymbol {
    public let name: String
    public let inheritedTypes: [Class]
}

public struct Property: SwiftSymbol, Codable {
    public let name: String
    public let type: String

    public init(name: String, type: String) {
        self.name = name
        self.type = type
    }
}

public struct Function: SwiftSymbol {
    public let name: String
    public let parameters: [String:String]
    public let returns: String
}

public struct ComponentStruct: SwiftSymbol, Codable {
    public let name: String
    public var properties: [Property] 

    public init(name: String, properties: [Property]) {
        self.name = name
        self.properties = properties
    }
}

public struct JobClass: SwiftSymbol, Codable {
    public let name: String
    public var properties: [Property]

    public init(name: String, properties: [Property]) {
        self.name = name
        self.properties = properties
    }
}
