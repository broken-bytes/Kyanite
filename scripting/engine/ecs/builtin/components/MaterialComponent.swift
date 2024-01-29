@Component public struct MaterialComponent: Codable {
    @Reference(type: Material.self)
    public var material: String
}