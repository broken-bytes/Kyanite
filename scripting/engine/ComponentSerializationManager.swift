import Foundation

public class _ComponentSerializationManager {
    public static let shared = _ComponentSerializationManager()

    private var serializers: [String: (any Hashable) -> [String: Any]] = [:]
    private var deserializers: [String: ([String: Any]) -> any Hashable] = [:]

    private init() {}

    public func _registerSerializer(name: String, serializer: @escaping (any Hashable) -> [String: Any]) {
        _ComponentSerializationManager.shared.serializers[name] = serializer
    }

    public func _serialize(component: any Hashable, with serializer: String) -> [String: Any] {
        guard let serializer = _ComponentSerializationManager.shared.serializers[serializer] else {
            fatalError("No serializer found named \(serializer)")
        }

        return serializer(component)
    }

    public func _registerDeserializer(name: String, deserializer: @escaping ([String: Any]) -> any Hashable) {
        _ComponentSerializationManager.shared.deserializers[name] = deserializer
    }

    public func _deserialize(data: [String: Any], with deserializer: String) -> any Hashable {
        guard let deserializer = _ComponentSerializationManager.shared.deserializers[deserializer] else {
            fatalError("No deserializer found named \(deserializer)")
        }

        return deserializer(data)
    }
}