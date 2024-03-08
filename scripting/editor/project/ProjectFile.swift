struct ProjectFile: Codable {
    let name: String
    let version: String
    let icon: String
    let scenes: [SceneReference]
}