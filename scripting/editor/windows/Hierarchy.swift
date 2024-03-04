import Native
import EditorNative
import KyaniteEngine

class Hierarchy: EditorWindow {
    public override var title: String { "Hierarchy" }
    private var selectedObject: UInt64? = nil
    private var showLabel: Bool = false
    private var entities: [Entity] = []
    
    override init() {
        super.init()
        EventSystem.shared.subscribe(to: EntityLifetimeEvent.self) { entity in 
            if entity.isAlive {
                self.entities.append(entity.entity)
            } else {
                self.entities.removeAll { $0.id == entity.entity.id }
            }
        }
    }

    override func onDraw() {
        for entity in entities {
            if EditorNativeCore.shared.button(entity.name) {
                selectedObject = entity.id
            }
        }
    }
}