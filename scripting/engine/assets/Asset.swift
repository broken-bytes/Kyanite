import Foundation

public class AnyAsset {
    var uuid: String
    var native: Any
    
    init(uuid: String, native: Any) {
        self.uuid = uuid
        self.native = native
    }

    func asType<T>() -> Asset<T> {
        return Asset<T>(uuid: uuid, native: native as! T)
    }
}

public protocol AssetProtocol {
    func eraseToAnyAsset() -> AnyAsset
}

public class Asset<T>: AssetProtocol {
    var uuid: String
    var native: T
    
    init(uuid: String, native: T) {
        self.uuid = uuid
        self.native = native
    }

    public func eraseToAnyAsset() -> AnyAsset {
        return AnyAsset(uuid: uuid, native: native)
    }
}