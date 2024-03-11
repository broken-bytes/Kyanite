#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

import Foundation

class List: Command {
    public let keyword = "ls"
    var subcommands: [Command] = []

    public func run(input: [String]) -> DataStructure {
        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        guard 
            let pathPtr = _getcwd(ptr, 1024),
            let path = String(cString: pathPtr, encoding: .utf8)
        else {
            return EmptyData()
        }
        let fm = FileManager.default

        var items: [Any] = []
        do {
            let items = try fm.contentsOfDirectory(atPath: path)
            var entries: [DataTableEntry] = []

            let dateFormatter = DateFormatter()
            dateFormatter.dateFormat = "MMM dd,yyyy"
            for item in items {
                let attributes = try FileManager.default.attributesOfItem(atPath: path.appending("/\(item)"))
                let fileSize = attributes[FileAttributeKey.size] as? UInt64
                let modifyDate = attributes[FileAttributeKey.modificationDate] as? Date

                let nameField = DataField(name: "name", value: item, type: .text)
                let sizeField = DataField(name: "size", value: String(fileSize ?? 0), type: .number)
                let modifyField = DataField(name: "size", value: (modifyDate ?? Date()), type: .number)
                let entry = DataTableEntry(fields: [nameField, sizeField, modifyField])

                entries.append(entry)
            }
            return DataTable(fields: entries)

        } catch {
            print(error)
        }

        return EmptyData()
    }
}