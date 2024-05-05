import Foundation

@Component 
public struct MeshComponent {
    public var mesh: Mesh = .box

    public init(mesh: Mesh) {
        self.mesh = mesh
    }
}