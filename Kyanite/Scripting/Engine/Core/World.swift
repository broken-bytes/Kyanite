public struct World {
    public static var activeWorld: World!

    var entities: [Entity] = []
    var systems: [System] = []

    public init(_ name: String, _ funcs: (() -> Entity)...) {
        World.activeWorld = self
        for ent in funcs {
            ent()
        }
    }

    internal mutating func addEntity(entity: Entity) -> World {
        entities.append(entity)

        return self
    }

    internal mutating func addEntity(name: String, @EntityBuilder components: () -> ComponentContent) -> World {
        let entity = Entity(name, components: components)
        entities.append(entity)

        return self
    }

    internal mutating func addSystem(system: System) -> World {
        systems.append(system)
        return self
    }
}

public func WithEntity(_ name: String, @EntityBuilder components: () -> ComponentContent) -> Entity {
    let entity = Entity(name: name)
    EntityBuilder.processEntity(entity: entity, components: components)
    return entity
}

