import Foundation

public extension ECS {

    struct ComponentData<A>: Sequence, IteratorProtocol {        

        // The count of this Data Sequence
        var count: Int
        // Make the Pools var, so we can add new pools if new archetypes are added
        private var pools: [ArchetypePool] = []
        // Store the sizes of each pool so we can query the correct pool index faster in subscript
        private var sizes: [Int] = []  
        private var currentIndex: Int = 0

        internal init(for archetype: BitSet) {
            for arch in archetypes {
                if arch.bitMask.hasAtLeast(other: archetype) {
                    pools.append(archetypePools[arch.bitMask]!)
                }
            }

            sizes = pools.compactMap { $0.size }
            count = sizes.reduce(0, +)
        }

        public subscript(_ index: Int) -> UnsafeMutablePointer<A> {
            get {
                var currentSize = sizes[0]
                var poolIndex = 0
                for size in sizes.dropFirst() {
                    if index >= currentSize {
                        currentSize += size
                        poolIndex += 1
                        continue
                    }
                    
                    break
                }
                
                return pools[poolIndex].component(of: A.self, at: poolIndex > 0 ? index - currentSize : index)
            } 
        }

        public mutating func next() -> UnsafeMutablePointer<A>? {
            if currentIndex >= count {
                return nil
            } else {
                defer { currentIndex += 1 }
                return self[currentIndex]
            }
        }
    }
}