#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

import Foundation

@main
struct Editor {
    static let repl = REPL()

    static func main() -> Void {
        signal(SIGINT) { 
            sigIntHandler(sig: $0)
        }

        while true {
            repl.printDir()

            if let command = repl.takeInput() {
                repl.eval(command: command)
            }
        }
    }
}

func sigIntHandler(sig: Int32) {
    signal(sig, SIG_IGN)
    signal(SIGINT, sigIntHandler)
    Editor.repl.handleSigInt()
}