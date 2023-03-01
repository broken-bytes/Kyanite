import Foundation

extension ECS {

    internal class ArchetypePool {

        // The raw memory block. Untyped raw so we can store any data
        internal var memory: UnsafeMutableRawPointer
        // The archetype that this pool uses
        internal let archetype: Archetype
        // The current index of the storage
        internal var currentIndex: Int = 0
        // The current size of the pool
        internal var size: Int = 0
        // The current capacity
        internal var capacity: Int = 1
        // Currently deleted entity ids. We use this so we do not have to move entities when a delete before end happens.
        internal var freeIds: [Int] = []
        // The index to Type mapping
        internal let componentOrder: [Any.Type]

        internal init(_ archetype: Archetype) {
            self.archetype = archetype
            memory = UnsafeMutableRawPointer.allocate(byteCount: archetype.size, alignment: MemoryLayout<UInt8>.alignment)
            var order: [Any.Type] = []

            var index: UInt8 = 0
            for x: UInt16 in 0..<archetype.bitMask.count {
                if archetype.bitMask.checkBit(at: x) {
                    if let compid = ComponentIdMapping.shared.mappings[UInt16(Math.pow(base: 2, raised: x))] {
                        order.append(compid)
                        index += 1
                    } else {
                        print(x)
                    }
                }
            }
            
            self.componentOrder = order
        }

        internal func add(components: [Any]) -> Int {
            let lock = DispatchSemaphore(value: 1)
            var index = -1

            if !freeIds.isEmpty {
                index = freeIds.removeLast() 
            } else {
                index = currentIndex
                currentIndex += 1
                size += 1
            }

            var byteOffsetCurrent = index * archetype.size

            components.sorted(by: { t0, t1 in
                ComponentIdMapping.shared.mappings.first(where: { $0.value == type(of: t0) })!.key <
                ComponentIdMapping.shared.mappings.first(where: { $0.value == type(of: t1) })!.key
            }).forEach { component in 
                withUnsafePointer(to: component) { ptr in 
                    let compSize = 
                        ComponentIdMapping.shared.sizes[ComponentIdMapping.shared.mappings.first(
                            where: { 
                                $0.value == type(of: component)
                            })!.key]!
                    memory.advanced(by: byteOffsetCurrent)
                        .copyMemory(
                            from: ptr, 
                            byteCount: compSize
                        )
                    byteOffsetCurrent += compSize
                }
            }

            if size >= capacity {
                resize()
            }

            lock.signal()

            return index
        }

        internal func remove(entity: Int) {
            freeIds.append(entity)
        }

        internal func component<T>(of type: T.Type, at index: Int) -> UnsafeMutablePointer<T> {
            var offset = 0
            var componentIndex = 0
            
            for x in 0..<componentOrder.count {
                if componentOrder[x] == T.self {
                    componentIndex = x
                    break
                }
            }
            
            if componentIndex != 0 {
                for x in 0..<componentIndex {
                    offset += archetype.sizes[x]
                }
            }

            return memory.advanced(by: (archetype.size * index) + offset).assumingMemoryBound(to: T.self)
        }
        
        internal func offsetFor<T>(_ type: T.Type) -> Int {
            var offset = 0
            var componentIndex = 0
            
            for x in 0..<componentOrder.count {
                if componentOrder[x] == T.self {
                    componentIndex = x
                    break
                }
            }
            
            if componentIndex != 0 {
                for x in 0..<componentIndex {
                    offset += archetype.sizes[x]
                }
            }
            
            return offset
        }
        
        /// Returns the size of a single entity for this archetype
        internal func sizeOf() -> Int {
            archetype.sizes.reduce(0, +)
        }

        private func resize() {
            let lock = DispatchSemaphore(value: 1)
            let newSize = capacity * 2
            let newPtr = UnsafeMutableRawPointer.allocate(byteCount: newSize * archetype.size, alignment: MemoryLayout<UInt8>.alignment)
            newPtr.copyMemory(from: memory, byteCount: capacity * archetype.size)
            capacity = newSize

            memory.deallocate()
            memory = newPtr
            lock.signal()
        }
    }
}
