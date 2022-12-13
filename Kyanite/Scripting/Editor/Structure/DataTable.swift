public enum DataTableFieldType {
    case text
    case number
    case timestamp
    case bool
}

public struct DataTableField {
    let title: String
    let type: DataTableFieldType
    let value: String
}

public struct DataTableEntry {
    let fields: [DataTableField]
}

public struct DataTable {
    let fields: [DataTableField]
}