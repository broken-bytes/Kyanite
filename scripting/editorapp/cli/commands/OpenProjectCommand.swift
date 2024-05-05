struct OpenProjectCommand: Command {
    var keyword: String = "open"
    var subcommands: [Command] = []
    var shortDescription: String = "Open a project"
    var longDescription: String = "Open a project"

    func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }
        precondition(input.count >= 1, "Missing project file")

        let projectFile = input[0]
        // Open the project
        do {
            try ProjectManager.shared.openProject(at: projectFile)
        } catch {
            print("Failed to open project: \(error.localizedDescription)")
            return EmptyData()
        }

        return EmptyData()
    }
}