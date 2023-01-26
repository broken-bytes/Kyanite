import Core

internal typealias SetTexture = @convention(c) (UInt64, UnsafePointer<Int8>, UInt64) -> Void

public class Material {
    static let lib: Library = Core.Runtime.library
    static let setMaterialTexture: SetTexture = Material.lib.loadFunc(named: "Shader_SetMaterialTexture")
    internal let internalRefId: UInt64

    public convenience init() {
        self.init(internalRefId: 0)
    }

    public init(internalRefId: UInt64) {
        self.internalRefId = internalRefId
    } 

    public func setTexture(named: String, texture: UInt64) {
        named.withCString {
            Material.setMaterialTexture(self.internalRefId, $0, texture)
        }
    }
}