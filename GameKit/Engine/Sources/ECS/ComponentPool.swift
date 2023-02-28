fileprivate let maxEntityCount = 10000

internal class ComponentPool<T> {
    
    private let memory: UnsafeMutablePointer<T>

    init(type: T.Type) {
        memory = UnsafeMutablePointer.allocate(capacity: maxEntityCount)
    }

    subscript(index: Int) ->  UnsafeMutablePointer<T> {
        get {
            memory + index
        }
    }

    func initComponent(entityId: Int, value: T) {
        memory[entityId] = value
    }
}