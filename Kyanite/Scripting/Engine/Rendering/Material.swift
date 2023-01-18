public class Material {

    internal let internalRefId: UInt64

    public init() {
        internalRefId = 0
    }

    internal convenience init(internalRefId: UInt64) {
        self.init()
    } 
}