@resultBuilder
public struct SceneGroupBuilder {
    public static func buildBlock(_ nodes: SceneNode...) -> SceneNodeCollection {
        SceneNodeCollection(nodes: nodes)
    }
    // Allow for loops to be used in the DSL
    public static func buildArray(_ components: [SceneNodeCollection]) -> SceneNodeCollection {
        SceneNodeCollection(nodes: components.flatMap { $0.nodes })
    }
}