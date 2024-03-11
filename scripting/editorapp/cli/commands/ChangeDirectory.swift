#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class ChangeDirectory: Command {
    public let keyword = "cd"
    var subcommands: [Command] = []

    public func run(input: [String]) -> DataStructure {
        _chdir(input[1])

        return EmptyData()
    }
}