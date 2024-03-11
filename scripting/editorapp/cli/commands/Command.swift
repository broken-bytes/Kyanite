public protocol Command {
    var keyword: String { get }
    var subcommands: [Command] { get }

    func run(input: [String]) -> DataStructure
}