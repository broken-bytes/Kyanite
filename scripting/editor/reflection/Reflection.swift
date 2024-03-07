import KyaniteEngine
import WinSDK

public struct Reflection: Decodable {
    struct Property: Decodable {
        let index: Int
        let name: String
        let type: PropertyType
    }

    internal let type: String
    internal let fields: [Property]

    internal func getProperties<T: Component>(of type: T.Type) -> [Property] {
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
        case .vector2:
            return MemoryLayout<Vector2>.size
        case .vector3:
            return MemoryLayout<Vector3>.size
        case .vector4:
            return MemoryLayout<Vector4>.size
        case .quaternion:
            return MemoryLayout<Quaternion>.size
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

    internal func value<T>(for property: Property, from buffer: UnsafeRawPointer) -> T {
        // Get the offset of the property
        let offset = self.offset(of: property.name)
        OutputDebugStringA("Property: \(property.name)\n")
        OutputDebugStringA("Offset: \(offset)\n")
        OutputDebugStringA("Value Buffer: \(buffer)\n")
        // Now read the value from the buffer
        return buffer.advanced(by: offset).assumingMemoryBound(to: T.self).pointee
    }

    internal func buffer<T>(for property: Property, from buffer: UnsafeRawPointer) -> UnsafeMutablePointer<T> {
        let offset = self.offset(of: property.name)
        OutputDebugStringA("Property: \(property.name)\n")
        OutputDebugStringA("Pointer Buffer: \(buffer)\n")
        // Now read the value from the buffer
        let mutableBuffer = UnsafeMutablePointer(mutating: buffer.advanced(by: offset).assumingMemoryBound(to: T.self))
        OutputDebugStringA("Mutable buffer: \(mutableBuffer)\n")
        return mutableBuffer
    }
}
