import Foundation

public struct MeshData: Codable {
    public let vertices: StorableCollection<Vertex>
    public let indices: StorableCollection<UInt32>

    public init(vertices: StorableCollection<Vertex>, indices: StorableCollection<UInt32>) {
        self.vertices = vertices
        self.indices = indices
    }
}