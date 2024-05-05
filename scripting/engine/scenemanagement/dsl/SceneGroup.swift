public struct SceneGroup: SceneNode {
    var nodes: SceneNodeCollection

    init(@SceneGroupBuilder _ content: () -> SceneNodeCollection) {
        nodes = content()
    }

    internal init(collection: SceneNodeCollection) {
        self.nodes = collection
    }
}