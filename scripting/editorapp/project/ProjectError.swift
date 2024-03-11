enum ProjectError: Error {
    case unknown
    case projectFileNotFound
    case projectFileInvalid
    case projectCreationFailed(String)
    case invalidProject
    case invalidProjectName
    case invalidProjectPath
    case invalidProjectVersion
}