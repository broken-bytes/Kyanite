public extension ECS {

    typealias SystemIterFunc = (SystemIterator) -> Void

    class System {

        let id: UInt64
        let name: String
        let archetype: BitSet
        let multithreaded: Bool
        let function: SystemIterFunc

        internal init(id: UInt64, name: String, archetype: BitSet, multithreaded: Bool, function: @escaping SystemIterFunc) {
            self.id = id
            self.name = name
            self.archetype = archetype
            self.multithreaded = multithreaded
            self.function = function
        }
    }
}