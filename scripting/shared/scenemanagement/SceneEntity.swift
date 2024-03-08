import Foundation

public struct SceneEntity: Codable {
    let uuid: String
    let name: String
    let components: [SceneComponent]
}