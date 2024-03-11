import Foundation
@_implementationOnly import Native

public class Shader {
    internal let uuid: String
    internal let handle : UInt64

    init(uuid: String, handle : UInt64) {
        self.uuid = uuid
        self.handle = handle
    }

    deinit {
        NativeRendering.shared.unloadShader(handle: handle)
    }
}