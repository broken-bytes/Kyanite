import Foundation

public class Mesh {
    let internalRefId: UInt64

    public convenience init() {
        self.init(internalRefId: 0)
    }

    public init(internalRefId: UInt64) {
        self.internalRefId = internalRefId
    } 
}