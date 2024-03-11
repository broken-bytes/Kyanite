public protocol Scene {
    @SceneBuilder
    var content: [Entity] { get }
}


struct MyScene: Scene {
    var content: [Entity] {
        Entity {
            TransformComponent()
        }
    }
}