public struct World {
    public init(_ name: String, _ funcs: (() -> Entity)...) {
        for ent in funcs {
            ent()
        }
    }
}

public func WithEntity(_ name: String, @WorldBuilder components: () -> WorldContent) -> Entity {
    let entity = Entity(name: name)

    let componentsData = components()

    if let compSingle = componentsData as? ComponentEntry {
        entity.addComponent(component: compSingle.component)
    }

    if let tuple = componentsData as? any ContentTupleBase {
        let mirror = Mirror(reflecting: tuple.item)
        
        switch mirror.children.count {
            case 2:
                handleTwoComps(entity: entity, tuple: tuple)   
            case 3:
                handleThreeComps(entity: entity, tuple: tuple)   
            case 4:
                handleFourComps(entity: entity, tuple: tuple)   
            case 5:
                handleFiveComps(entity: entity, tuple: tuple)   
            case 6:
                handleSixComps(entity: entity, tuple: tuple)   
            case 7:
                handleSevenComps(entity: entity, tuple: tuple)   
            case 8:
                handleEightComps(entity: entity, tuple: tuple)                                                                          
            default:
                fatalError("Too large") 
        }
    }
    
    return entity
}

fileprivate func handleTwoComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
}

fileprivate func handleThreeComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
}

fileprivate func handleFourComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
}

fileprivate func handleFiveComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
    entity.addComponent(component: item.4)
}

fileprivate func handleSixComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
    entity.addComponent(component: item.4)
    entity.addComponent(component: item.5)
}

fileprivate func handleSevenComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
    entity.addComponent(component: item.4)
    entity.addComponent(component: item.5)
    entity.addComponent(component: item.6)
}

fileprivate func handleEightComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
    entity.addComponent(component: item.4)
    entity.addComponent(component: item.5)
    entity.addComponent(component: item.6)
    entity.addComponent(component: item.7)
}