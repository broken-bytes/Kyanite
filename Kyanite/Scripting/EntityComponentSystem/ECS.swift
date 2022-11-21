import Foundation

public class ECS { 
    var entities: [Entity] = []

    public static var shared = ECS()
    public var registerCallback: ((Entity) -> Void)!
    
    private init() {
        
    }

    public func register(entity: Entity) {
        entities.append(entity)       
        self.registerCallback?(entity) 
    }

    public func filter(types: [Component.Type]) -> [Entity] {
        if entities.isEmpty {
            return []
        }
        var results: [Entity] = []
        for entity in entities {
            var isValid = true
            for component in types {
                if !isValid  {
                    continue
                }
                var hasComponent = false
                for ec in entity.componentPtrs {
                    if type(of: ec.pointee) == component {
                        hasComponent = true
                        continue
                    }
                }
                if hasComponent {
                    isValid = true
                } else {
                    isValid = false
                }
            }

            if isValid {
                results.append(entity)
            }
        }

        return results
    }

    public func entity(by uuid: String) -> Entity? {
        return entities.first(where: { $0.uuid == uuid })
    }
}
