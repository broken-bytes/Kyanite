#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

import Foundation

public class REPL {
    public let commands: [Command] = [
        Clear(),
        PrintWorkingDirectory(),
        ChangeDirectory(),
        Exit(),
        List(),
        ProjectCommand()
    ]
    public var currentInput: String = ""

    public init() {
        clear()
        Console.default.print(
            color: .brightMagenta,
            background: .black,
            str:
"""
💎 Welcome to Kyanite Editor Shell. 💎\n\n
"""
)

        Console.default.print(color: .brightMagenta, background: .black, str: "Our")
        Console.default.print(color: .green, background: .black, str: " GitHub")
        Console.default.print(color: .brightMagenta, background: .black, str: " repository is at ")
        Console.default.print(color: .brightCyan, background: .black, str: "https://github.com/broken-bytes/Kyanite\n")

        Console.default.print(color: .brightMagenta, background: .black, str: "Visit our")
        Console.default.print(color: .green, background: .black, str: " Website")
        Console.default.print(color: .brightMagenta, background: .black, str: " at ")
        Console.default.print(color: .brightCyan, background: .black, str: "https://broken-bytes.dev\n\n")
    }

    public func update() {
        clear()
        printDir()
    }

    public func eval(command: String) {
        let keywords = command.components(separatedBy: " ")

        if let cmd = self.commands.first(where: { $0.keyword == keywords[0]}) {
            // If out command did output text, we head over to the next line
            let output = cmd.run(input: keywords)
            if !(output is EmptyData) {
                if let textOutput = output as? DataValue {
                    Console.default.print(color: .white, background: .black, str: "\(textOutput.value as! String)\n")
                } else if output is DataTable {
                    let table = output as! DataTable
                    for row in table.fields {
                        for field in row.fields {
                            Console.default.print(color: .white, background: .black, str: "\(field.value) ")
                        }
                        Console.default.print(color: .white, background: .black, str: "\n")
                    }
                }
            }
        } else {
            Console.default.print(color: .brightRed, background: .black, str: "Command not found\n")
        }
    }

    public func pipeline(input: DataStructure, command: [String]) -> DataStructure {
        return input
    }

    public func takeInput() -> String? {
        var line = readLine(strippingNewline: true)
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
        _getcwd(ptr, 1024);
        Console.default.print(color: .brightGreen, background: .black, str: "\(String(cString: ptr))")
        Console.default.print(color: .brightCyan, background: .black, str: "〉")
    }

    public func printIndicator() {
        Console.default.print(color: .brightCyan, background: .black, str: "⊙")
    }

    public func handleSigInt() {
        print("\n")
        printDir()
    }
}