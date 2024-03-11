public protocol CLICommand {
    var keyword: String { get }
    var subcommands: [CLICommand] { get }

    func execute(args: [String]) throws -> Int
}