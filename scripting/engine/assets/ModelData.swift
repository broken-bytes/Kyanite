import Foundation

public struct ModelData: Codable {
    public let meshes: StorableCollection<MeshData>

    public init(meshes: StorableCollection<MeshData>) {
        self.meshes = meshes
    }
}