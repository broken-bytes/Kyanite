#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

public class CLI {
    public static let shared = CLI()

    var repl = REPL()

    init() {
        SetUnhandledExceptionFilter { exception in
            print("Unhandled exception: \(exception)")
            return EXCEPTION_CONTINUE_SEARCH
        }

        signal(SIGINT) { _ in
            print("SIGINT")
            CLI.handleSigInt()
            // Return to the REPL, don't exit the program
        }
    }

    public func run(with args: [String]) {
        while true {
            repl.printDir()

            if let command = repl.takeInput() {
                repl.eval(command: command)
            }
        }
    }

    static func handleSigInt() {
        CLI.shared.repl.clear()
        CLI.shared.repl.printDir()
    }
}