class Clear: Command {
    public let keyword = "clear"
    var subcommands: [Command] = []

    public func run(input: [String]) -> DataStructure {
        var esc = UnicodeScalar(27)!
        Swift.print("\(esc)[2J");
        Swift.print("\(esc)[3J");

        return EmptyData()
    }
}