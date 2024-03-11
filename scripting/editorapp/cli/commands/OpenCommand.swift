class OpenCommand: Command {
    public let keyword = "open"
    var subcommands: [Command] = []

    public func run(input: [String]) -> DataStructure {
        return EmptyData()
    }
}