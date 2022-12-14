public enum DataTableFieldType {
    case text
    case number
    case timestamp
    case bool
    case size
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
    let fields: [DataTableEntry]
}

public func widthFor(field named: String) -> Int {
    let minWidth = named.count + 2 // Min Width is the title char count + 2 (1 Padding per side)

    return minWidth
}

public func makeTable(from items: [Any]) -> DataTable {
    for item in items {
        let mirror = Mirror(reflecting: item)

        for prop in mirror.children {
            print(prop)
        }
    }

    return DataTable(fields: [])
}
