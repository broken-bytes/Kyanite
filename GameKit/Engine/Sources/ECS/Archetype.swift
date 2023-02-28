public extension ECS {    

    struct Archetype {
    
        // The size of this archetype in bytes
        internal let size: Int
        // The components for this archetype
        internal let bitMask: BitSet
        // The size of each component
        internal let sizes: [Int]

        internal init(with bitMask: BitSet, sizes: [Int]) {
            self.sizes = sizes
            self.bitMask = bitMask
            size = sizes.reduce(0, +)
        }
    }
}
