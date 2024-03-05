import KyaniteEngine
import EditorNative
import Native
import WinSDK

class Inspector: EditorWindow {
    public override var title: String { "Inspector" }
    private var selectedObject: Entity? = nil
    
    override init() {
        super.init()
        EventSystem.shared.subscribe(to: EditorSelectionEvent.self) { event in
            guard var entity = event.selectedObject else {
                return
            }

            self.selectedObject = entity

            var components = entity.components
            //let reflection = ReflectionManager.shared.reflection(for: event.selectedObject)
        }
    }

    override func onDraw() {
        if let selectedObject = selectedObject {
            drawEntityInspector(for: selectedObject)
        }
    }

    private func drawEntityInspector(for entity: Entity) {
        for component in entity.components {
            let componentType = component.type
            let componentData = component.data

            // Use reflection to draw the inspector for the component
            guard let reflection = ReflectionManager.shared.reflection(for: componentType) else {
                // Draw a message saying that the component doesn't have a reflection
                EditorNativeCore.shared.label("Component \(componentType) doesn't have a reflection")
                return
            }

            for property in reflection.fields {
                // Draw a label for the property
                switch property.type {
                case .vector3:
                    drawVector3Field(
                        property.name, 
                        value: reflection.value(for: property, from: componentData)
                    )
                case .vector4:
                    drawVector4Field(
                        property.name, 
                        value: reflection.value(for: property, from: componentData)
                    )
                case .quaternion:
                    drawQuaternionField(
                        property.name, 
                        value: reflection.value(for: property, from: componentData)
                    )
                default:
                    break
                }
            }
        }
    }

    private func drawVector3Field(_ name: String, value: Vector3) {
        // Draw a label for the property
        EditorNativeCore.shared.label("\(name): \(value.x), \(value.y), \(value.z)")
    }

    private func drawVector4Field(_ name: String, value: Vector4) {
        // Draw a label for the property
        EditorNativeCore.shared.label("\(name): \(value.x), \(value.y), \(value.z), \(value.w)")
    }

    private func drawQuaternionField(_ name: String, value: Quaternion) {
        // Draw a label for the property
        EditorNativeCore.shared.label("\(name): \(value.x), \(value.y), \(value.z), \(value.w)")
    }
}