@_implementationOnly import Native
import KyaniteEngine

class Terminal: EditorWindow {
    public override var title: String { "Terminal" }
    private var lastCommands: [String] = []
    var currentCommand: UnsafeMutableBufferPointer<UInt8> = UnsafeMutableBufferPointer.allocate(capacity: 256)

    override init() {
        super.init()
        
    }

    override func onDraw() {
        if NativeImGui.shared.button("Clear") {
            lastCommands.removeAll()
        }
        for command in lastCommands {
            NativeImGui.shared.label(command)
        }

        // Draw the input field
        if NativeImGui.shared.text("##TerminalInput", text: currentCommand) {
            lastCommands.append(String(cString: currentCommand.baseAddress!))
            currentCommand = UnsafeMutableBufferPointer.allocate(capacity: 256)
        }
    }
}