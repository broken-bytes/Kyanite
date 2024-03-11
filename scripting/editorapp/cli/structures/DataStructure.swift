public protocol DataStructure {}

public struct EmptyData: DataStructure {}

public enum DataType {
    case text
    case number
    case timestamp
    case bool
    case size
    case table
}

public struct DataField {
    let name: String
    let value: Any
    let type: DataType
    let color: Color = .white
    let background: BackgroundColor = .black
}