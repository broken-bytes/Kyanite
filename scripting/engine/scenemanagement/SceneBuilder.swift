@resultBuilder
public struct SceneBuilder {
    public static func buildBlock(_ entities: Entity...) -> Scene {
        return Scene(
            entities: entities.map { $0 }
        )
    }
}