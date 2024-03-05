import KyaniteEngine

public class Reflection: Decodable {
    struct Property: Decodable {
        let index: Int
        let name: String
        let type: PropertyType
    }

    internal let type: String = ""
    internal let fields: [Property] = []

    init() {
        
    }

    func getProperties<T: Component>(of type: T) -> [Property] {
        fields
    } 

    internal static func size(of type: PropertyType) -> Int {
        switch type {
        case .int8:
            return MemoryLayout<Int8>.size
        case .int16:
            return MemoryLayout<Int16>.size
        case .int32:
            return MemoryLayout<Int32>.size
        case .int64:
            return MemoryLayout<Int64>.size
        case .float:
            return MemoryLayout<Float>.size
        case .double:
            return MemoryLayout<Double>.size
        default:
            return 0
        }
    }

    internal func offset(of property: String) -> Int {
        // We need to get the offset of the property in the buffer.
        // For that, we loop over the fields and sum the size of the types.
        
        // Get the property index
        guard 
            let index = fields.first(where: { $0.name == property })?.index
        else {
            return -1
        }

        // Get the offset
        var offset = 0
        for i in 0..<index {
            offset += Reflection.size(of: fields[i].type)
        }

        return offset
    }
}
