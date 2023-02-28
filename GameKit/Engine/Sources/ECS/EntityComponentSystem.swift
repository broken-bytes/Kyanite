public enum ECS {

    static internal var currentId: UInt64 = 0
    static internal var archetypes: [Archetype] = []
    static internal var archetypePools: [BitSet:ArchetypePool] = [:]

    static public func createWorld() -> World {
        World()
    }
}