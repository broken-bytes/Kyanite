public struct EntityLifetimeEvent: Event {
    public var type: Any.Type { EntityLifetimeEvent.self }
    public var entity: Entity
    public var isAlive: Bool
    
    public init(entity: Entity, isAlive: Bool) {
        self.entity = entity
        self.isAlive = isAlive
    }
}