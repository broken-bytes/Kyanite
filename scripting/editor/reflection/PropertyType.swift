enum PropertyType: String, Decodable {
    case int8 = "i8"
    case int16 = "i16"
    case int32 = "i32"
    case int64 = "i64"
    case float = "f32"
    case double = "f64"
    case bool = "bool"
    case vector2 = "Vector2"
    case vector3 = "Vector3"
    case vector4 = "Vector4"
    case quaternion = "Quaternion"

    // Allow this to be constructed from a string for json decoding
    init?(rawValue: String) {
        switch rawValue {
        case "i8":
            self = .int8
        case "i16":
            self = .int16
        case "i32":
            self = .int32
        case "i64":
            self = .int64
        case "f32":
            self = .float
        case "f64":
            self = .double
        case "bool":
            self = .bool
        case "Vector2":
            self = .vector2
        case "Vector3":
            self = .vector3
        case "Vector4":
            self = .vector4
        case "Quaternion":
            self = .quaternion
        default:
            return nil
        }
    }
}