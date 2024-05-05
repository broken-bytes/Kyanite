class OpenCommand: Command {
    public let keyword = "open"
    var subcommands: [Command] = []
    let shortDescription: String = "Open a file"
    let longDescription: String = "Open a file"

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }

        return EmptyData()
    }
}