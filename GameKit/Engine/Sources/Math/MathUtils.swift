public enum Math {

    public static func pow(base: UInt8, raised: Int) -> UInt8 {
        var val = base
        
        if raised == 0 {
            return 1
        }
        
        for _ in 1..<raised {
            val += base
        }

        return val
    }

    public static func pow(base: UInt8, raised: UInt16) -> UInt8 {
        var val = base
        
        if raised == 0 {
            return 1
        }
        
        for _ in 1..<raised {
            val += base
        }

        return val
    }

    public static func pow(base: Int, raised: Int) -> Int {
        var val = base
        
        if raised == 0 {
            return 1
        }
        
        for _ in 1..<raised {
            val += base
        }

        return val
    }
}
