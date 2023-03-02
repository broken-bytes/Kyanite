public extension ECS {
    
    struct SystemIterator {
        
        // The count of this Data Sequence
        public let count: Int
        // The offset at which the Iterator starts
        private var offset: Int
        // The index of the iterator
        private var index: Int
        // The components this iterator takes
        private var archetype: BitSet
        // Make the Pools var, so we can add new pools if new archetypes are added
        private var pools: [ArchetypePool] = []
        // Store the sizes of each pool so we can query the correct pool index faster in subscript
        private var sizes: [Int] = []

        public init(for archetype: BitSet, starting at: Int = 0, count: Int) {
            self.archetype = archetype
            index = at

            self.count = count
            for arch in archetypes {
                if arch.bitMask.hasAtLeast(other: archetype) {
                    pools.append(archetypePools[arch.bitMask]!)
                }
            }
            
            sizes = pools.compactMap { $0.size }
            offset = at
        }

        public func dataFor<T>(_ type: T.Type, at index: Int) -> UnsafeMutablePointer<T> {
            let actualIndex = index + offset
            var currentSize = sizes[0]
            var poolIndex = 0
            for x in 1..<sizes.count {
                if actualIndex >= currentSize {
                    currentSize += sizes[x]
                    poolIndex += 1
                    continue
                }
                
                break
            }

            return pools[poolIndex].component(of: T.self, at: poolIndex > 0 ? actualIndex - currentSize : actualIndex)
        }
    }
}
