public protocol Command {
    var shortDescription: String { get }
    var longDescription: String { get }
    var keyword: String { get }
    var subcommands: [Command] { get }

    func run(input: [String]) -> DataStructure
}

extension Command {
    public func help(input: [String]) -> Bool {
        if input.contains("--help") {
            Console.default.print(color: .brightMagenta, background: .black, str: "\(shortDescription)\n")

            for subcommand in subcommands {
                Console.default.print(color: .brightMagenta, background: .black, str: " - \(subcommand.keyword): \(subcommand.shortDescription)\n")
            }

            return true
        }

        return false
    }
}