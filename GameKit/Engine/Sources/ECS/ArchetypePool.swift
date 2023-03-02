import Foundation

extension ECS {

    internal class ArchetypePool {

        fileprivate static let alignment = 8
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
        // The offsets for each component
        internal let componentOffsets: [Int]
        // The stride for each component. Is equal to the largest component for speed
        internal let stride: Int
        // The stride for each component. Is equal to the largest component for speed
        internal let strideTotal: Int

        internal init(_ archetype: Archetype) {
            memory = UnsafeMutableRawPointer.allocate(byteCount: archetype.size, alignment: ArchetypePool.alignment)
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
            
            var offsets: [Int] = []
            
            var targetComponent = 0
            var offset = 0
    
            for x in 0..<order.count {
                if targetComponent != 0 {
                    for y in 0..<x {
                        offset += archetype.sizes[y]
                    }
                }
                
                offsets.append(offset)
                targetComponent += 1
                offset = 0
            }
            
            // Get the theoretical stride first
            let stride = archetype.sizes.sorted(by: { $0 > $1 }).first!
            // Now get the next 8 increment for faster pointer access
            var actualStride = 0
            
            repeat {
                actualStride += ArchetypePool.alignment
            } while actualStride < stride
            
            self.componentOrder = order
            self.componentOffsets = offsets
            self.stride = actualStride
            self.strideTotal = self.stride * componentOrder.count
            self.archetype = archetype
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

            var byteOffsetCurrent = index * strideTotal

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
                            byteCount: stride
                        )
                    byteOffsetCurrent += stride
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

        @inlinable
        internal func component<T>(of type: T.Type, at index: Int) -> UnsafeMutablePointer<T> {
            var componentIndex = 0
            
            for x in 0..<componentOrder.count {
                if componentOrder[x] == T.self {
                    componentIndex = x
                    break
                }
            }
            
            let offset = componentIndex * stride


            return memory.advanced(by: (strideTotal * index) + offset).assumingMemoryBound(to: T.self)
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
            let newPtr = UnsafeMutableRawPointer.allocate(byteCount: newSize * strideTotal, alignment: ArchetypePool.alignment)
            newPtr.copyMemory(from: memory, byteCount: capacity * strideTotal)
            capacity = newSize

            memory.deallocate()
            memory = newPtr
            lock.signal()
        }
    }
}
