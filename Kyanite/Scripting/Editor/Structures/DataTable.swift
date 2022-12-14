public struct DataTableEntry {
    let fields: [DataField]
}

public struct DataTable: DataStructure {
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
