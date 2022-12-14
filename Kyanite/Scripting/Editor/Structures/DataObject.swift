public struct DataObjectProperty {
    let name: String
    let value: Any
    let type: DataType
}

public struct DataObject {
    let props: [DataObjectProperty]
}