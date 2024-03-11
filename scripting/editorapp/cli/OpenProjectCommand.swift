struct OpenProjectCommand: CLICommand {
    var keyword: String = "open"
    var subcommands: [CLICommand] = []

    func execute(args: [String]) throws -> Int {
        precondition(args.count > 3, "Missing project file")

        let projectFile = args[3]
        // Open the project
        do {
            try ProjectManager.shared.openProject(at: projectFile)
        } catch {
            print("Failed to open project: \(error.localizedDescription)")
            return -1
        }

        return 0
    }
}