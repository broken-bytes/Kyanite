public protocol Scene {
    associatedtype Body: SceneNode
    @SceneBuilder var content: Body { get }
    
    func onDidLoad()
}

extension Scene {
    public func onDidLoad() {}
}

public protocol SceneNode {}

extension Entity: SceneNode {}

public struct TestScene: Scene {
    public init() {}

    public var content: some SceneNode {
        SceneGroup {
            Entity("Player") {
                TransformComponent(position: .zero, rotation: .identity, scale: .zero)
                MeshComponent(mesh: .box)
            }
            Entity("NPC") {
                TransformComponent(position: .zero, rotation: .identity, scale: .zero)
            }
            SceneGroup {
                Entity("Another NPC") {
                    TransformComponent(position: .zero, rotation: .identity, scale: .zero)
                }
                ForEach(0..<5) { index in
                    Entity("Enemy \(index)") {
                        TransformComponent(position: .zero, rotation: .identity, scale: .zero)
                    }
                }
            }
        }
    }

    public func onDidLoad() {
        print("Scene loaded")
    }
}