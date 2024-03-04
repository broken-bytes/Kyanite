import KyaniteEngine

struct ItemSelectedEvent: Event {
    var type: Any.Type { ItemSelectedEvent.self }
    var item: UInt64
    var isSelected: Bool = false

    init(item: UInt64, isSelected: Bool = false) {
        self.item = item
        self.isSelected = isSelected
    }
}