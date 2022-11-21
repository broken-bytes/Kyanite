import Foundation

public struct AudioClip {
    public let data: Data
    public let length: Int

    public init(data: Data, length: Int) {
        self.data = data
        self.length = length
    }
}