struct OpenProjectCommand: Command {
    var keyword: String = "open"
    var subcommands: [Command] = []

    func run(input: [String]) -> DataStructure {
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