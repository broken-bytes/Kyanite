import Foundation

public struct CreateProjectCommand: CLICommand {
    public var keyword: String = "create"
    public var subcommands: [CLICommand] = []

    public init() {
    }

    public func execute(args: [String]) throws -> Int {
        precondition(args.count > 3, "Missing project name")

        let projectName = args[3]

        // Create the project
        do {
            try ProjectManager.shared.createProject(name: projectName, path: FileManager.default.currentDirectoryPath)
        } catch {
            print("Failed to create project: \(error.localizedDescription)")
            return -1
        }
        
        return 0
    }
}