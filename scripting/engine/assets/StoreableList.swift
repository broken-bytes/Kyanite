/// A list that can be stored in a file
import Foundation

public struct StorableCollection<Element: Codable>: Collection {
    public let count: Int
    private let elements: [Element]

    public init(elements: [Element] = []) {
        self.count = elements.count
        self.elements = elements
    }

    // Collection conformance
    public var startIndex: Int { 0 }
    public var endIndex: Int { elements.index(after: count - 2) }

    public func index(after i: Int) -> Int {
        elements.index(after: i)
    }

    public subscript(position: Int) -> Element {
        elements[position]
    }
}

extension StorableCollection: Codable {
    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        elements = try container.decode([Element].self)
        count = elements.count
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(elements)
    }
}