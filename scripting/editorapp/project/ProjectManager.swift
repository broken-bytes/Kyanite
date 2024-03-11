import Foundation
import KyaniteEditor

class ProjectManager {
    static let shared = ProjectManager()

    private var projectFolder: String = ""
    private var buildFolder: String {
        return "\(projectFolder)/.build"
    }
    private var cacheFolder: String {
        return "\(projectFolder)/.cache"
    }
    private var assembliesFolder: String {
        return "\(cacheFolder)/assemblies"
    }
    private var blobsFolder: String {
        return "\(cacheFolder)/blobs"
    }
    private var metadataFolder: String {
        return "\(cacheFolder)/metadata"
    }
    private var logsFolder: String {
        return "\(projectFolder)/.logs"
    }
    private var contentFolder: String {
        return "\(projectFolder)/content"
    }
    private var srcFolder: String {
        return "\(projectFolder)/src"
    }
    
    func createProject(name: String, path: String) throws {
        let projectFile = ProjectFile(name: name, version: "1.0.0", icon: "icon.png", scenes: [])
        guard let projectFileData = try? JSONEncoder().encode(projectFile) else {
            throw ProjectError.unknown
        }

        guard 
            FileManager.default.createFile(atPath: "\(path)/\(name).kyproj", contents: projectFileData, attributes: nil) 
        else {
            throw ProjectError.projectCreationFailed("Failed to create project file")
        }

        projectFolder = "\(path)"

        do {
            // Create the project folder structure
            try FileManager.default.createDirectory(atPath: buildFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: cacheFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: assembliesFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: blobsFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: metadataFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: logsFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: contentFolder, withIntermediateDirectories: true, attributes: nil)

            try FileManager.default.createDirectory(atPath: srcFolder, withIntermediateDirectories: true, attributes: nil)
        } catch {
            throw ProjectError.projectCreationFailed(error.localizedDescription)
        }
    }

    func openProject(at path: String) throws -> ProjectFile {
        guard let projectFileData = FileManager.default.contents(atPath: path) else {
            throw ProjectError.projectFileNotFound
        }

        guard let projectFile = try? JSONDecoder().decode(ProjectFile.self, from: projectFileData) else {
            throw ProjectError.projectFileInvalid
        }

        projectFolder = path.deletingLastPathComponent

        kyaniteeditormain(at: projectFolder)

        return projectFile
    }
}