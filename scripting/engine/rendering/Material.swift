import Foundation
@_implementationOnly import Native

public class Material {
    public var name: String
    public var shader: Shader?
    public var properties: [String: Any] = [:]

    required init(name: String, uuid: String) {
        self.name = name
    }

    public func setFloat(name: String, value: Float) {
        properties[name] = value
    }

    public func setVector2(name: String, value: Vector2) {
        properties[name] = value
    }

    public func setVector3(name: String, value: Vector3) {
        properties[name] = value
    }

    public func setVector4(name: String, value: Vector4) {
        properties[name] = value
    }

    public func setTexture(name: String, value: Texture) {
        properties[name] = value
    }

    public func setInt(name: String, value: Int) {
        properties[name] = value
    }

    public func setBool(name: String, value: Bool) {
        properties[name] = value
    }
}