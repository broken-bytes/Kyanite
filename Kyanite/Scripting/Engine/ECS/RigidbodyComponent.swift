public struct RigidBodyComponent: Component {
    public let isStatic: Bool
    public let mass: Float

    public init(mass: Float, isStatic: Bool) {
        self.mass = mass
        self.isStatic = isStatic
    } 
}