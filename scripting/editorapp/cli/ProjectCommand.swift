struct ProjectCommand: CLICommand {
    let name = "project"
    let shortDescription = "Manage projects"
    let longDescription = "Manage projects"
    let subcommands: [CLICommand] = [CreateProjectCommand(), OpenProjectCommand()]
    
    var keyword: String {
        return name
    }

    func execute(args: [String]) throws -> Int {
        guard let targetCommand = subcommands.first(where: { $0.keyword == args[2] }) else {
            fatalError("Invalid subcommand for project command, expected one of: \(subcommands.map { $0.keyword })")
        }

        return try targetCommand.execute(args: args)
    }
}