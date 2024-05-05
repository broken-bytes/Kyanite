import Foundation

public struct CreateProjectCommand: Command {
    public var keyword: String = "create"
    public var subcommands: [Command] = []
    public var shortDescription: String = "Create a new project"
    public var longDescription: String = "Create a new project"

    public init() {
    }

    public func run(input: [String]) -> DataStructure {
        guard !help(input: input) else { return EmptyData() }
        precondition(input.count >= 1, "Missing project name")

        let projectName = input[0]

        // Create the project
        do {
            try ProjectManager.shared.createProject(name: projectName, path: FileManager.default.currentDirectoryPath)
        } catch {
            print("Failed to create project: \(error.localizedDescription)")
            return EmptyData()
        }

        return EmptyData()
    }
}