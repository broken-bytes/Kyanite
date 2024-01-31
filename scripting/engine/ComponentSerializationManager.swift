public class _ComponentSerializationManager {
    public static let shared = _ComponentSerializationManager()

    private var serializers: [String: (any Component) -> [String: Any]] = [:]
    private var deserializers: [String: ([String: Any]) -> any Component] = [:]

    private init() {}

    public func _registerSerializer(name: String, serializer: @escaping (any Component) -> [String: Any]) {
        _ComponentSerializationManager.shared.serializers[name] = serializer
    }

    public func _serialize(component: any Component, with serializer: String) -> [String: Any] {
        guard let serializer = _ComponentSerializationManager.shared.serializers[serializer] else {
            fatalError("No serializer found named \(serializer)")
        }

        return serializer(component)
    }

    public func _registerDeserializer(name: String, deserializer: @escaping ([String: Any]) -> any Component) {
        _ComponentSerializationManager.shared.deserializers[name] = deserializer
    }

    public func _deserialize(data: [String: Any], with deserializer: String) -> any Component {
        guard let deserializer = _ComponentSerializationManager.shared.deserializers[deserializer] else {
            fatalError("No deserializer found named \(deserializer)")
        }

        return deserializer(data)
    }
}