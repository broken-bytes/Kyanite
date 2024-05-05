struct ProjectCommand: Command {
    let name = "project"
    let shortDescription = "Manage projects"
    let longDescription = "Manage projects"
    let subcommands: [Command] = [CreateProjectCommand(), OpenProjectCommand()]

    var keyword: String {
        return name
    }

    func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }

        guard let targetCommand = subcommands.first(where: { $0.keyword == input[1] }) else {
            fatalError("Invalid subcommand for project command, expected one of: \(subcommands.map { $0.keyword })")
        }

        return targetCommand.run(input: Array(input.dropFirst(2)))
    }
}