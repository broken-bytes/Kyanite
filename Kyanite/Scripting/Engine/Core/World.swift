public struct World {
    public init(_ name: String, _ funcs: (() -> Entity)...) {
        for ent in funcs {
            ent()
        }
    }
}

public func WithEntity(_ name: String, @EntityBuilder components: () -> ComponentContent) -> Entity {
    let entity = Entity(name: name)

    let componentsData = components()

    if let compSingle = componentsData as? ComponentEntry {
        entity.addComponent(component: compSingle.component)
    }

    if let tuple = componentsData as? any ContentTupleBase {
        let mirror = Mirror(reflecting: tuple.item)
        
        switch mirror.children.count {
            case 2:
                EntityBuilder.handleTwoComps(entity: entity, tuple: tuple)   
            case 3:
                EntityBuilder.handleThreeComps(entity: entity, tuple: tuple)   
            case 4:
                EntityBuilder.handleFourComps(entity: entity, tuple: tuple)   
            case 5:
                EntityBuilder.handleFiveComps(entity: entity, tuple: tuple)   
            case 6:
                EntityBuilder.handleSixComps(entity: entity, tuple: tuple)   
            case 7:
                EntityBuilder.handleSevenComps(entity: entity, tuple: tuple)   
            case 8:
                EntityBuilder.handleEightComps(entity: entity, tuple: tuple)                                                                          
            default:
                fatalError("Too large") 
        }
    }
    
    return entity
}

