class Engine {
    var time: Float = 0 

    init() {
    }

    func start() {
        _ComponentSerializationManager.shared._registerSerializer(name: "TransformComponent") { component in
            guard let component = component as? TransformComponent else {
                fatalError()
            }
            return [
                "position": component.position,
                "rotation": component.rotation,
                "scale": component.scale
            ]
        }

        print(_ComponentSerializationManager.shared._serialize(component: TransformComponent(), with: "TransformComponent"))

        _ComponentSerializationManager.shared._registerDeserializer(name: "TransformComponent") { data in
            var transform = TransformComponent()
            transform.position = data["position"] as! Vector3
            transform.rotation = data["rotation"] as! Quaternion
            transform.scale = data["scale"] as! Vector3

            return transform
        }

        print(_ComponentSerializationManager.shared._deserialize(data: [
            "position": Vector3(x: 1, y: 2, z: 3),
            "rotation": Quaternion(x: 2, y: 3, z: 4, w: 5),
            "scale": Vector3(x: 1, y: 2, z: 3)
        ], with: "TransformComponent"))

        Bridge_Engine_Init(nil, nil, Mode(0), nil)
        _ComponentRegistry.shared._register(TransformComponent.self)
        _ComponentRegistry.shared._register(MeshComponent.self)
        let renderSystem = RenderSystem()
        let entity = Entity(name: "Test")
        entity.addComponent(TransformComponent.self)
        entity.addComponent(MeshComponent.self)
        while true {
            var measure = ContinuousClock().measure {
                // Update all native modules first each frame
                InputManager.shared.update()
                Bridge_Engine_Update(time)
                // Update the swift internal systems
            }
            // Attoseconds to milliseconds
            time = Float(Double(measure.components.attoseconds) / 10000000000000000.0)
        }
    }
}