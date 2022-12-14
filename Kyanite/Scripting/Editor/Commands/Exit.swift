#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class Exit: Command {
    public let keyword = "exit"


    public func run(input: [String]) -> DataStructure {
        exit(0)
        return EmptyData()
    }
}