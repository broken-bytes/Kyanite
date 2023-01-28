@resultBuilder
public struct EntityBuilder {
    public static func buildBlock<C>(_ component: C) -> ComponentContent where C : Component {
        return ComponentEntry(component)
    }

    public static func buildBlock<C0, C1>(_ c0: C0, _ c1: C1) -> ComponentTuple<(C0, C1)> where C0 : Component, C1 : Component {
        return ComponentTuple((c0, c1))
    }

    public static func buildBlock<C0, C1, C2>(_ c0: C0, _ c1: C1, _ c2: C2) -> ComponentTuple<(C0, C1, C2)> where C0 : Component, C1 : Component, C2 : Component {
      return ComponentTuple((c0, c1, c2))
    }

    public static func buildBlock<C0, C1, C2, C3>(_ c0: C0, _ c1: C1, _ c2: C2, _ c3: C3) -> ComponentTuple<(C0, C1, C2, C3)> where C0 : Component, C1 : Component, C2 : Component, C3: Component {
      return ComponentTuple((c0, c1, c2, c3))
    }

    public static func buildBlock<C0, C1, C2, C3, C4>(_ c0: C0, _ c1: C1, _ c2: C2, _ c3: C3, _ c4: C4) -> ComponentTuple<(C0, C1, C2, C3, C4)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component {
      return ComponentTuple((c0, c1, c2, c3, c4))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5>(_ c0: C0, _ c1: C1, _ c2: C2, _ c3: C3, _ c4: C4, _ c5: C5) -> ComponentTuple<(C0, C1, C2, C3, C4, C5)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component, C5: Component {
      return ComponentTuple((c0, c1, c2, c3, c4, c5))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5, C6>(_ c0: C0, _ c1: C1, _ c2: C2, _ c3: C3, _ c4: C4, _ c5: C5, _ c6: C6) -> ComponentTuple<(C0, C1, C2, C3, C4, C5, C6)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C3: Component, C3: Component, C4: Component, C5: Component, C6: Component {
      return ComponentTuple((c0, c1, c2, c3, c4, c5, c6))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5, C6, C7>(_ c0: C0, _ c1: C1, _ c2: C2, _ c3: C3, _ c4: C4, _ c5: C5, _ c6: C6, _ c7: C7) -> ComponentTuple<(C0, C1, C2, C3, C4, C5, C6, C7)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component, C5: Component, C6: Component, C7: Component {
      return ComponentTuple((c0, c1, c2, c3, c4, c5, c6, c7))
    }
}

public protocol ComponentContent {}

public struct ComponentEntry: ComponentContent {
    let component: Component 

    public init(_ component: Component) {
      self.component = component
    }
}

public protocol ContentTupleBase {
    associatedtype Item

    var item: Item { get }
}

public struct ComponentTuple<Tuple>: ComponentContent, ContentTupleBase {
  public typealias Item = Tuple
  let tuple: Tuple
  public var item: Tuple { self.tuple }

  public init(_ tuple: Tuple) {
    self.tuple = tuple
  }
}

extension EntityBuilder {
  static internal func handleTwoComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
}

static internal func handleThreeComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
}

static internal func handleFourComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
}

static internal func handleFiveComps(entity: Entity, tuple: any ContentTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
    entity.addComponent(component: item.0)
    entity.addComponent(component: item.1)
    entity.addComponent(component: item.2)
    entity.addComponent(component: item.3)
    entity.addComponent(component: item.4)
}

static internal func handleSixComps(entity: Entity, tuple: any ContentTupleBase) {
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

static internal func handleSevenComps(entity: Entity, tuple: any ContentTupleBase) {
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

static internal func handleEightComps(entity: Entity, tuple: any ContentTupleBase) {
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

static internal func processEntity(entity: Entity, @EntityBuilder components: () -> ComponentContent) {
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
}
}
