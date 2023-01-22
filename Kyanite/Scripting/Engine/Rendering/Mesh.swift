import Foundation

public class Mesh: Resource {

    public init() {
        super.init(uuid: UUID().uuidString, refId: 0)
    }

    internal convenience init(internalRefId: UInt64) {
        self.init()
    } 
}