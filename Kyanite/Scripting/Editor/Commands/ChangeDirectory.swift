#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class ChangeDirectory: Command {
    public let keyword = "cd"


    public func run(input: [String]) -> DataStructure {
        chdir(input[1])

        return EmptyData()
    }
}