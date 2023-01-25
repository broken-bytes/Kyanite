@resultBuilder
public struct SystemBuilder {
    public static func buildBlock<C>(_ component: C.Type) -> SystemContent where C : Component {
        return SystemEntry(component)
    }

    public static func buildBlock<C0, C1>(_ c0: C0.Type, _ c1: C1.Type) -> SystemTuple<(C0.Type, C1.Type)> where C0 : Component, C1 : Component {
        return SystemTuple((c0, c1))
    }

    public static func buildBlock<C0, C1, C2>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type)> where C0 : Component, C1 : Component, C2 : Component {
        return SystemTuple((c0, c1, c2))
    }

    public static func buildBlock<C0, C1, C2, C3>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type, _ c3: C3.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type, C3.Type)> where C0 : Component, C1 : Component, C2 : Component, C3: Component {
        return SystemTuple((c0, c1, c2, c3))
    }

    public static func buildBlock<C0, C1, C2, C3, C4>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type, _ c3: C3.Type, _ c4: C4.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type, C3.Type, C4.Type)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component {
        return SystemTuple((c0, c1, c2, c3, c4))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type, _ c3: C3.Type, _ c4: C4.Type, _ c5: C5.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component, C5: Component {
        return SystemTuple((c0, c1, c2, c3, c4, c5))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5, C6>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type, _ c3: C3.Type, _ c4: C4.Type, _ c5: C5.Type, _ c6: C6.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type, C6.Type)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C3: Component, C3: Component, C4: Component, C5: Component, C6: Component {
        return SystemTuple((c0, c1, c2, c3, c4, c5, c6))
    }

    public static func buildBlock<C0, C1, C2, C3, C4, C5, C6, C7>(_ c0: C0.Type, _ c1: C1.Type, _ c2: C2.Type, _ c3: C3.Type, _ c4: C4.Type, _ c5: C5.Type, _ c6: C6.Type, _ c7: C7.Type) -> SystemTuple<(C0.Type, C1.Type, C2.Type, C3.Type, C4.Type, C5.Type, C6.Type, C7.Type)> where C0 : Component, C1 : Component, C2 : Component, C3: Component, C4: Component, C5: Component, C6: Component, C7: Component {
        return SystemTuple((c0, c1, c2, c3, c4, c5, c6, c7))
    }
}

public protocol SystemContent {}

public struct SystemEntry: SystemContent {
    let component: Component.Type

    public init(_ component: Component.Type) {
      self.component = component
    }
}

public protocol SystemTupleBase {
    associatedtype Item

    var item: Item { get }
}

public struct SystemTuple<Tuple>: SystemContent, SystemTupleBase {
  public typealias Item = Tuple
  let tuple: Tuple
  public var item: Tuple { self.tuple }

  public init(_ tuple: Tuple) {
    self.tuple = tuple
  }
}

extension SystemBuilder {
  static internal func handleTwoComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
}

static internal func handleThreeComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }

}

static internal func handleFourComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }

}

static internal func handleFiveComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }

}

static internal func handleSixComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }

}

static internal func handleSevenComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
}

static internal func handleEightComps(tuple: any SystemTupleBase) {
    guard let item = tuple.item as? (any Component, any Component, any Component, any Component, any Component, any Component, any Component, any Component) else { 
        fatalError("Types unexpectedly don't match") 
    }
}

static internal func processSystem(@EntityBuilder components: () -> SystemContent) {
   let componentsData = components()

    if let compSingle = componentsData as? SystemEntry {
    }

    if let tuple = componentsData as? any SystemTupleBase {
        let mirror = Mirror(reflecting: tuple.item)
        
        switch mirror.children.count {
            case 2:
                SystemBuilder.handleTwoComps(tuple: tuple)   
            case 3:
                SystemBuilder.handleThreeComps(tuple: tuple)   
            case 4:
                SystemBuilder.handleFourComps(tuple: tuple)   
            case 5:
                SystemBuilder.handleFiveComps(tuple: tuple)   
            case 6:
                SystemBuilder.handleSixComps(tuple: tuple)   
            case 7:
                SystemBuilder.handleSevenComps(tuple: tuple)   
            case 8:
                SystemBuilder.handleEightComps(tuple: tuple)                                                                          
            default:
                fatalError("Too large") 
        }
    }
}
}