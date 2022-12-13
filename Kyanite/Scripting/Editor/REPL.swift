#if os(Windows)
import WinSDK
#endif

import Foundation

public class REPL {
    public let commands: [Command] = [
        Clear(), PrintWorkingDirectory()
    ]
    public var currentInput: String = ""

    public init() {
        clear()
    }

    public func update() {
        clear()
        printIndicator()
        printDir()
    }

    public func eval(command: String) {
        let keywords = command.components(separatedBy: "")

        printIndicator()
        printDir()

        if let cmd = self.commands.first(where: { $0.keyword == keywords[0]}) {
            cmd.run(input: keywords)
        }
    }

    public func takeInput() -> String? {
        var line = readLine(strippingNewline: false)
        return line
    }

    public func printChar(col: Int, row: Int, character: Character) {

    }

    public func clear() {
        Console.default.clear()
    }

    // - MARK: Print Helpers
    public func printDir() {
        var ptr = UnsafeMutablePointer<UInt8>.allocate(capacity: 1024)
        getcwd(ptr, 1024);
        Console.default.print(color: .red, background: .black, str: "\(String(cString: ptr))")
        Console.default.print(color: .brightCyan, background: .black, str: ">")
    }

    public func printIndicator() {
        Console.default.print(color: .brightCyan, background: .black, str: "⊙")
    }

    public func handleSigInt() {
        print("\n")
        printIndicator()
        printDir()
    }
}