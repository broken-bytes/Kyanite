import Core
import Math
import Userland

@_cdecl("AssemblyComponentsInit") public func initComponents() {

    ComponentFactory.shared.componentFactories["HealthComponent"] = { data in 
        var component = HealthComponent()

        component.health = ComponentFactory.shared.parse(property: data.first(where: { $0.name == "health"})!) as! Float
        return component}
    ComponentFactory.shared.componentFactories["AIComponent"] = { data in 
        var component = AIComponent()

        component.isHostile = ComponentFactory.shared.parse(property: data.first(where: { $0.name == "isHostile"})!) as! Bool

        component.radius = ComponentFactory.shared.parse(property: data.first(where: { $0.name == "radius"})!) as! Float
        return component}}