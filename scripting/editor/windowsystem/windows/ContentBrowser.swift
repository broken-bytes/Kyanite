@_implementationOnly import Native
import Foundation

class ContentBrowser: EditorWindow {
    public override var title: String { "Content Browser" }
    
    private var showLabel: Bool = false
    private var contentManager: ContentManager
    
    init(contentManager: ContentManager) {
        self.contentManager = contentManager
        super.init()
        contentManager.onChange.append { (file, change) in

        }
    }

    override func onDraw() {
        if NativeImGui.shared.treeNode("Content") {
            // Draw the content browser. Use the filesytem to list the files in the content directory
            for file in try! FileManager.default.contentsOfDirectory(atPath: contentManager.path) {
                // Draw a tree if the file is a directory
                if let isDirectory = try? FileManager.default.contentsOfDirectory(atPath: contentManager.path + "/" + file) {
                    if isDirectory.count > 0 {
                        if NativeImGui.shared.treeNode(file) {
                            for subfile in isDirectory {
                                NativeImGui.shared.label(subfile)
                            }
                            NativeImGui.shared.treePop()
                        }
                    }
                } else {
                    NativeImGui.shared.label(file)
                }
            }

            NativeImGui.shared.treePop()
        } 
    }
}