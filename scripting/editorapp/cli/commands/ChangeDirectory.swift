#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class ChangeDirectory: Command {
    public let keyword = "cd"
    var subcommands: [Command] = []
    let shortDescription: String = "Change the current working directory"
    let longDescription: String = "Change the current working directory"

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }
        _chdir(input[1])

        return EmptyData()
    }
}