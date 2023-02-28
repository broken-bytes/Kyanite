public extension ECS {
    
    struct SystemIterator {
        
        // The count of this Data Sequence
        public private(set) var count: Int
        // The index of the iterator
        private var index: Int
        // The components this iterator takes
        private var archetype: BitSet
        // Make the Pools var, so we can add new pools if new archetypes are added
        private var pools: [ArchetypePool] = []

        public init(for archetype: BitSet, starting at: Int = 0) {
            self.archetype = archetype
            index = at

            for arch in archetypes {
                if arch.bitMask.hasAtLeast(other: archetype) {
                    pools.append(archetypePools[arch.bitMask]!)
                }
            }

            count = pools.reduce(0, { $0 + $1.size})
        }

        public func dataFor<T>(_ type: T.Type) -> ComponentData<T> {
            ComponentData<T>(for: archetype)
        }
    }
}
