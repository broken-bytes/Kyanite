@_implementationOnly import Native
import Foundation

class ContentBrowser: EditorWindow {
    static let fileViewItemSize: Float = 64
    public override var title: String { "Content Browser" }

    private var showLabel: Bool = false
    private var contentManager: ContentManager
    private var currentFolder: String = EditorEnvironment.default.contentFolder

    init(contentManager: ContentManager) {
        self.contentManager = contentManager
        super.init()
        contentManager.onChange.append { (file, change) in

        }
    }

    override func onDraw() {
        // Draw the file structure
        // First, get the available width so we know how many columns to draw per row
        let width = NativeImGui.shared.getAvailableWidth()
        // Get the max items per row by dividing the width by the size of the file view item
        let maxItemsPerRow = Int(width / Self.fileViewItemSize)

        let columnCount = maxItemsPerRow
        var currentRow = 0
        // Get the number of rows by dividing the number of files by the max items per row
        if NativeImGui.shared.beginTable("Files", count: Int32(columnCount), flags: 0) {
            guard let files = try? FileManager.default.contentsOfDirectory(atPath: currentFolder) else {
                NativeImGui.shared.endTable()
                return
            }

            for (index, file) in files.enumerated() {
                let fullPath = currentFolder + "/" + file
                if index % columnCount == 0 {
                    NativeImGui.shared.tableNextRow()
                    currentRow += 1
                }

                guard let attributes = try? FileManager.default.attributesOfItem(atPath: fullPath) else {
                    NativeImGui.shared.endTable()
                    return
                }

                NativeImGui.shared.tableSetColumnIndex(Int32(index % columnCount))
                drawFileButton(type: attributes[FileAttributeKey.type] as? String ?? "Unknown", name: fullPath)
            }

            NativeImGui.shared.endTable()
        }
    }

    private func drawFileButton(type: String, name: String) {
        guard
            let lastPathComponent = name.split(separator: "/").last
        else {
            return
        }
        var icon_size: Float = 64.0  // Define icon size
        var text_height: Float = 24.0 // Get text line height
        var total_height = icon_size + text_height  // Total content height
        if NativeImGui.shared.button("###\(name)", width: &icon_size, height: &total_height) {
            if type == "NSFileTypeDirectory" {
                currentFolder = name
            }
        }

        var cursorPosY: Float = 0.0
        NativeImGui.shared.getCursorPos(&cursorPosY)

        NativeImGui.shared.setCursorPos(cursorPosY - total_height)
        NativeImGui.shared.icon(type == "NSFileTypeDirectory" ? "\u{e067}" : "\u{e061}")

        var label = ""

        if name.count > 6 {
            label = String(lastPathComponent.prefix(6) + "...")
        } else {
            label = String(lastPathComponent)
        }

        NativeImGui.shared.label(label)
    }
}