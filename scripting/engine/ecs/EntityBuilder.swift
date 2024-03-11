@resultBuilder
public struct EntityBuilder {
    public static func buildBlock(_ components: any Component...) -> [any Component] {
        return components.map { $0 }
    }
}