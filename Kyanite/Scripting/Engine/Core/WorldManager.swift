@resultBuilder
public struct WorldBuilder {
    public static func buildBlock<C>(_ component: C) -> WorldContent where C : Component {
        return ComponentEntry(component)
    }

    public static func buildBlock<C0, C1>(_ c0: C0, _ c1: C1) -> ComponentTuple<(C0, C1)> where C0 : Component, C1 : Component {
        return ComponentTuple((c0, c1))
    }

    public static func buildBlock<C0, C1, C2>(_ c0: C0, _ c1: C1, _ c2: C2) -> ComponentTuple<(C0, C1, C2)> where C0 : Component, C1 : Component, C2 : Component {
      return ComponentTuple((c0, c1, c2))
    }
}

public protocol WorldContent {}

public struct ComponentEntry: WorldContent {
    let component: Component 

    public init(_ component: Component) {
      self.component = component
    }
}


public protocol ContentTupleBase {
    associatedtype Item

    var item: Item { get }
}

public struct ComponentTuple<Tuple>: WorldContent, ContentTupleBase {
  public typealias Item = Tuple
  let tuple: Tuple
  public var item: Tuple { self.tuple }

  public init(_ tuple: Tuple) {
    self.tuple = tuple
  }
}
