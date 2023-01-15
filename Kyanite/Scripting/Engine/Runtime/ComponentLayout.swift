internal struct ComponentLayout {

    internal enum PropSize: UInt8 {
        case tiny       // 8 bit
        case small      // 16 bit
        case regular    // 32 bit 
        case large      // 64 bit
        case vec2       // 64 * 2
        case vec3       // 64 * 3
        case vec4       // 64 * 4
        case arr        // 32 bits for size prop + 64 bits for data ptr, so 96 bits
    }

    internal var layout: [PropSize] = []

    internal mutating func push(prop: PropSize) {
        layout.append(prop)
    }
}