#if os(Windows)
import WinSDK
#endif

import Foundation

class List: Command {
    public let keyword = "ls"


    public func run(input: [String]) -> Bool {
        let ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        let pathPtr = getcwd(ptr, 1024)
        let fm = FileManager.default
        let path = Bundle.main.resourcePath!

        var items: [Any] = []
        do {
            let items = try fm.contentsOfDirectory(atPath: path)
            var entries: [DataTableEntry] = []

            let dateFormatter = DateFormatter()
            dateFormatter.dateFormat = "MMM dd,yyyy"
            for item in items {
                let attributes = try FileManager.default.attributesOfItem(atPath: path.appending("/\(item)"))
                let fileSize = attributes[FileAttributeKey.size] as! UInt64
                let modifyDate = attributes[FileAttributeKey.modificationDate] as? Date

                let nameField = DataTableField(title: "name", type: .text, value: item)
                let sizeField = DataTableField(title: "size", type: .number, value: String(fileSize))
                let modifyField = DataTableField(title: "size", type: .number, value: (modifyDate ?? Date()))
                let entry = DataTableEntry(fields: [nameField, sizeField, modifyDate])

                entries.append(entry)
            }
        } catch {
            print(error)
        }

        return true
    }
}