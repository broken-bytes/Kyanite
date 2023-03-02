import Foundation

public extension ECS {
        
    struct ComponentData<A>: Sequence, IteratorProtocol {
        
        internal struct ComponentDataPoolInfo {
            let type: Any.Type
            let offset: Int
            let size: UInt8
        }

        // The count of this Data Sequence
        var count: Int
        // The offset at which to start. Used for multithreading
        private var offset: Int
        // Make the Pools var, so we can add new pools if new archetypes are added
        private var pools: [ArchetypePool] = []
        // Store the sizes of each pool so we can query the correct pool index faster in subscript
        private var sizes: [Int] = []
        // The current Index: Used for sequencing
        private var currentIndex: Int = 0

        internal init(for archetype: BitSet, offset: Int) {
            for arch in archetypes {
                if arch.bitMask.hasAtLeast(other: archetype) {
                    pools.append(archetypePools[arch.bitMask]!)
                }
            }

            sizes = pools.compactMap { $0.size }
            count = sizes.reduce(0, +)
            self.offset = offset
        }

        public subscript(_ index: Int) -> UnsafeMutablePointer<A> {
            get {
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

                return pools[poolIndex].component(of: A.self, at: poolIndex > 0 ? actualIndex - currentSize : actualIndex)
            } 
        }
        
        public func at(index: Int) -> UnsafeMutablePointer<A> {
            var currentSize = sizes[0]
            var poolIndex = 0
            for x in 1..<sizes.count {
                if index >= currentSize {
                    currentSize += sizes[x]
                    poolIndex += 1
                    continue
                }
                
                break
            }

            return pools[poolIndex].component(of: A.self, at: poolIndex > 0 ? index - currentSize : index)
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
