extension ECS {

    public final class Entity {
        
        // The global Id of this entity
        // Some bits are reserved for functionality:
        // - 63 Bit: Deleted/Not Deleted
        // - 62 Bit: Enabled/Disabled
        // - 61-58 bits: Type: 
        //      - 0000 = Entity         0
        //      - 0001 = Component      1    
        //      - 0010 = System         2
        //      - 0100 = Tag            4
        //      - 1000 = Relationship   8 
        public internal(set) var id: UInt64
        // The name of this entity
        public internal(set) var name: String
        // Archetype Bitmask for this entity
        internal var bitMask: BitSet
        // The index of this entity inside of the pool it is contained in
        private var poolIndex: Int

        internal init(id: UInt64, name: String, bitMask: BitSet, components: [Codable]) {
            self.id = id
            self.name = name
            self.bitMask = bitMask
            
            if archetypePools[bitMask] == nil {
                print("No pool found with bitmask \(bitMask)")
                let arch = Archetype(
                    with: self.bitMask, 
                    sizes: components.compactMap { item in
                        ComponentIdMapping.shared.sizes[ComponentIdMapping.shared.mappings.first(where: { $0.value == type(of: item) })!.key]
                    })
                archetypes.append(arch)
                archetypePools[bitMask] = ArchetypePool(arch)

            }
            
            poolIndex = archetypePools[bitMask]!.add(components: components)
            for arch in archetypes {
                if arch.bitMask.hasExactly(other: self.bitMask) {
                    arch.add(components)
                }
            }
        }

        public func hasComponent<T: Codable>(_ component: T.Type) throws -> Bool {
            guard let id = ComponentIdMapping.shared.mappings.first(where: { $0.value == T.self}) else {
                throw ECSError.componentNotRegistered(type: T.self, message: "Trying to query a component that does not exist")
            }
            
            return bitMask.checkBit(at: id.key)
        }

        public func getComponent<T: Codable>(_ component: T.Type) throws -> UnsafeMutablePointer<T>? {
            guard let id = ComponentIdMapping.shared.mappings.first(where: { $0.value == T.self}) else {
                throw ECSError.componentNotRegistered(type: T.self, message: "Trying to query a component that does not exist")
            }
            
            // Shift the number to the right by 1 bit until it is zero and increment index
            // Our components are stored as power of 2 Ids, but the BitMask takes input from 0-max, thus we need to cast our component id to the actual bit inside of the mask
            var bitIndex: UInt16 = 0
            var shifted = id.key
            while shifted > 0 {
                shifted = shifted >> 1
                if shifted > 0 {
                    bitIndex += 1
                }                
            }

            if !bitMask.checkBit(at: bitIndex) {
                throw ECSError.componentNotFound(type: T.self, message: "The component requested was not present on this entity")
            }

            return archetypePools[bitMask]!.component(of: T.self, at: poolIndex)
        }
    }
}
