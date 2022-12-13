#if os(Windows)
import WinSDK
#endif

class Exit: Command {
    public let keyword = "exit"


    public func run(input: [String]) -> Bool {
        exit(0)
        return false
    }
}