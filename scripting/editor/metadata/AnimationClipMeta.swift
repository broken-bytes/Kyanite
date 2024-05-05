struct AnimationClipMeta: Codable {
    let name: String
    let takeName: String
    let firstFrame: Int
    let lastFrame: Int
    let loop: Bool
    let mirror: Bool
}