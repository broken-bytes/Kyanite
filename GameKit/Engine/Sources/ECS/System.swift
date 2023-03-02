internal protocol SystemData {
    
    associatedtype Data
    var id: UInt64 { get }
    var name: String { get }
    var archetype: ECS.BitSet { get }
    var multithreaded: Bool { get }
    var function: ([Data]) -> Void { get }
    var typedef: Data.Type { get }
    var components: [Codable.Type] { get }
}

public extension ECS {

    typealias SystemIterFunc = (SystemIterator) -> Void

    class System<T>: SystemData {
        
        typealias Data = T
        let id: UInt64
        let name: String
        let archetype: BitSet
        let multithreaded: Bool
        let function: ([Data]) -> Void
        let typedef: T.Type
        let components: [Codable.Type] = []
        
        internal init(id: UInt64, name: String, archetype: BitSet, multithreaded: Bool, function: @escaping ([Data]) -> Void) {
            self.id = id
            self.name = name
            self.archetype = archetype
            self.multithreaded = multithreaded
            self.function = function
            self.typedef = T.self
        }
        
        internal func populateComponentData() -> [Data] {
            var data: [Data] = []
            for arch in archetypes {
                if arch.bitMask.hasAtLeast(other: self.archetype) {
                    for x in 0..<arch.size {
                        arch.component(, at: <#T##Int#>)
                    }
                }
            }
        }
    }
}
