import Foundation

public struct Vertex: Codable {
    public let position: Vector3
    public let normal: Vector3
    public let uv: Vector2

    public init(position: Vector3, normal: Vector3, uv: Vector2) {
        self.position = position
        self.normal = normal
        self.uv = uv
    }
}