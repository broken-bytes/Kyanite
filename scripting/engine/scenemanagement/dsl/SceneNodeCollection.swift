public struct SceneNodeCollection {
    var nodes: [any SceneNode] = []

    internal init(nodes: [any SceneNode]) {
        self.nodes = nodes
    }
}