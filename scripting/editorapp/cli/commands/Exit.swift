#if os(Windows)
import WinSDK
#elseif os(macOS)
import Darwin
#else
#endif

class Exit: Command {
    public let keyword = "exit"
    var subcommands: [Command] = []
    let shortDescription: String = "Exit the application"
    let longDescription: String = "Exit the application"

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }
        exit(0)
    }
}