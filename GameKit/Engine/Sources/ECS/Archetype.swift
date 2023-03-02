public extension ECS {    

    class Archetype {
    
        // The size of this archetype in bytes
        internal let size: Int
        // The components for this archetype
        internal let bitMask: BitSet
        // The size of each component
        internal let sizes: [Int]
        // The types ordered for faster access
        internal let typeOrder: [Codable.Type]
        // All components
        internal var data: ContiguousArray<any Codable>
        // The actual count of entities for this archetype
        internal private(set) var count: Int

        internal init(with bitMask: BitSet, sizes: [Int]) {
            self.sizes = sizes
            self.bitMask = bitMask
            size = sizes.reduce(0, +)
            data = []
            
            var order: [Codable.Type] = []
            
            for x: UInt16 in 0..<bitMask.count {
                if bitMask.checkBit(at: x) {
                    if let compid = ComponentIdMapping.shared.mappings[UInt16(Math.pow(base: 2, raised: x))] {
                        order.append(compid)
                    }
                }
            }
            
            self.typeOrder = order
            self.count = data.count / sizes.count
        }
        
        public func add(_ components: [Codable]) {
            data.append(contentsOf: components)
            self.count = data.count / sizes.count
        }
        
        public func component<T: Codable>(_ type: T.Type, at index: Int) -> UnsafeMutablePointer<T>? {
            for x in 0..<typeOrder.count {
                if typeOrder[x] == T.self {
                    return withUnsafeMutablePointer(to: &data) {
                        ($0 + (index * typeOrder.count)) + x as! UnsafeMutablePointer<T>
                    }
                }
            }
            
            fatalError("Type not found")
        }
    }
}
