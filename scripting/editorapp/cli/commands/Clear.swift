class Clear: Command {
    public let keyword = "clear"
    var subcommands: [Command] = []
    let shortDescription: String = "Clear the terminal screen"
    let longDescription: String = "Clear the terminal screen"

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }

        var esc = UnicodeScalar(27)!
        Swift.print("\(esc)[2J");
        Swift.print("\(esc)[3J");

        return EmptyData()
    }
}