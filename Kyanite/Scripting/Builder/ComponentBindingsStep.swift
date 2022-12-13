import Foundation


struct ComponentBindingProperty: Codable {
    public var name: String
    public var type: String
}
struct ComponentBinding: Codable {
    public var name: String
    public var properties: [ComponentBindingProperty]
}
public class ComponentBindingStep: GeneratorStep {
    func setUp() {
        
    }

    func run() {
        let path: String = CommandLine.arguments[1]

        guard let metadata = FileManager.default.contents(atPath: path.appending("/Build/components.metadata")) else {
             fatalError("Metadata for components missing")
        }

        let decoder = JSONDecoder()
        do {
            let componentData = try decoder.decode([ComponentBinding].self, from: metadata)
            
            var componentsFactoryCode = 
"""
import Core
import Math
import Userland

@_cdecl("AssemblyComponentsInit") public func initComponents() {

"""
            for component in componentData {
                componentsFactoryCode.append(
"""

    ComponentFactory.shared.componentFactories["\(component.name)"] = { data in 
        var component = \(component.name)()

"""
)

for property in component.properties {
    componentsFactoryCode.append(
"""

        component.\(property.name) = ComponentFactory.shared.parse(property: data.first(where: { $0.name == "\(property.name)"})!) as! \(property.type)

"""
)
}

componentsFactoryCode.append(
"""
        return component
"""
)

componentsFactoryCode.append("}")
            }
componentsFactoryCode.append("}")

        FileManager.default.createFile(atPath: path.appending("/Cache/_generated_Components.swift"), contents: componentsFactoryCode.data(using: .utf8))

        } catch {
            print(error.localizedDescription)
        }
    }

    func cleanUp() {
        
    }

    
}