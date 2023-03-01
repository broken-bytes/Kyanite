import Foundation

public extension ECS {

    struct BitSet {

        // Store values in one byte packs. Raw pointer memory 
        var memory: UnsafeMutablePointer<UInt8>
        // The size for one element - 1 for index 0
        static let elementSize: Int = (MemoryLayout<UInt8>.size * 8) - 1
        // The size it takes to store on byte
        static let byteSize: Int = MemoryLayout<UInt8>.size * 8
        // The lnegth of this BitSet
        public private(set) var count: UInt16

        public init(length: UInt16) {
            let memCount = Int(length / UInt16(BitSet.byteSize)) + 1
            memory = UnsafeMutablePointer<UInt8>.allocate(capacity: memCount)
            memory.initialize(repeating: 0, count: memCount)
            memory.initialize(repeating: 0, count: 1)

            self.count = length
        }

        public func checkBit(at index: UInt16) -> Bool {
            let byteIndex = UInt16(floor(Float(index) / Float(BitSet.elementSize)))
            let bitIndex: UInt16 = index - UInt16(BitSet.byteSize) * byteIndex
            
            
            return (memory.advanced(by: Int(byteIndex)).pointee & UInt8(Math.pow(base: 2, raised: bitIndex))) > 0
        }

        public func setBit(at index: UInt16) -> Void {
            let overflow = floor(Float(index) / Float(BitSet.elementSize))
            let byteIndex = UInt16(overflow)
            let bitIndex: UInt16 = UInt16(index) - UInt16(BitSet.byteSize) * byteIndex

            memory.advanced(by: Int(byteIndex)).pointee |= UInt8(Math.pow(base: 2, raised: bitIndex))
        }

        public func unsetBit(at index: UInt16) -> Void {
            let byteIndex = UInt16(floor(Float(index) / Float(BitSet.elementSize)))
            let bitIndex: UInt16 = index - UInt16(BitSet.byteSize) * byteIndex

            memory.advanced(by: Int(byteIndex)).pointee &= ~UInt8(Math.pow(base: 2, raised: bitIndex))
        }

        public func hasAtLeast(other: BitSet) -> Bool {
            for x in 0..<count {
                if other.checkBit(at: x) && !self.checkBit(at: x) {
                    return false
                }
            }

            return true
        }

        public func hasExactly(other: BitSet) -> Bool {
            for x in 0..<count {
                if other.checkBit(at: x) != self.checkBit(at: x) {
                    return false
                }
            }

            return true
        }
    }
}

extension ECS.BitSet: Hashable {

    public func hash(into hasher: inout Hasher) {
        let bytes = (count / UInt16(ECS.BitSet.elementSize)) + 1
        
        for x in 0..<bytes {
            let byte = memory.advanced(by: Int(x)).pointee
            hasher.combine(byte)
        }
    }
}

extension ECS.BitSet: Equatable {

    public static func == (lhs: ECS.BitSet, rhs: ECS.BitSet) -> Bool {
        lhs.hasExactly(other: rhs)        
    }
}

extension ECS.BitSet: CustomStringConvertible {
    
    public var description: String {
        let bytes = (count / UInt16(ECS.BitSet.elementSize)) + 1

        var str = ""
        for x in 0..<bytes {
            let byte = memory.advanced(by: Int(x)).pointee
            str.append(String(byte, radix: 2))
        }

        return str
    }
}
