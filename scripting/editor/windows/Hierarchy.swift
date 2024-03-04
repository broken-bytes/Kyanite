import Native
import EditorNative
import KyaniteEngine
import WinSDK

class Hierarchy: EditorWindow {
    public override var title: String { "Hierarchy" }
    private var showLabel: Bool = false
    private var entities: [Entity] = []
    
    override init() {
        super.init()
        EventSystem.shared.subscribe(to: EntityLifetimeEvent.self) { event in 
            if event.isAlive {
                OutputDebugStringA("Parent: \(event.entity.parent?.name ?? "nil")\n")
                // If there are no parents, then it's a root entity
                guard let parent = event.entity.parent else {
                    return
                }

                // We don't want editor entities to be shown in the hierarchy
                if parent != EditorEnvironment.shared.editorParent {
                    self.entities.append(event.entity)
                    print(event.entity.components)
                }
            } else {
                self.entities.removeAll { $0.id == event.entity.id }
            }
        }
    }

    override func onDraw() {
        for entity in entities {
            if EditorNativeCore.shared.button(entity.name) {
                EditorEnvironment.shared.selectedObject = entity.id
            }
        }
    }
}