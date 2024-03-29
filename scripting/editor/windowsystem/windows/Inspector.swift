import KyaniteEngine
@_implementationOnly import Native

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
                NativeImGui.shared.label("Component \(componentType) doesn't have a reflection")
                return
            }

            // Draw the component's name
            NativeImGui.shared.label("\(componentType)")

            for property in reflection.fields {
                // Draw a label for the property
                switch property.type {
                case .vector3:
                    Float3Field(label: property.name, buffer: reflection.buffer(for: property, from: componentData))
                    .draw()
                case .vector4:
                    Float4Field(label: property.name, buffer: reflection.buffer(for: property, from: componentData))
                    .draw()
                case .quaternion:
                    Float4Field(label: property.name, buffer: reflection.buffer(for: property, from: componentData))
                    .draw()
                default:
                    break
                }
            }

            NativeImGui.shared.separator()
        }
    }
}