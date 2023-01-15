public class WorldManager {
    public static let shared = WorldManager()

    public private(set) var currentWorld: World?

    private init() {

    }

    public func loadWorld(name: String) async throws-> World {
        let world = World()
        currentWorld = world

        return world
    }
}