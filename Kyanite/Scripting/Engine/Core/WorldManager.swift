public class WorldManager {
    public static let shared = WorldManager()

    internal private(set) var currentWorld: World?

    private init() {

    }

    public func loadWorld(name: String) async throws -> Void {
    }
}