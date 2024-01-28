// This will be generated at some point, but for now it's manually created

@main
public struct Kyanite {
    public static func main() throws {
        print("Hello World")
        Bridge_Engine_Init(nil, nil, Mode(0), nil)
        registerTransformComponent()
        let entity = Entity(name: "Test1")
        entity.addComponent(TransformComponent())
        let entity2 = Entity(name: "Test2")

        let engine = Engine()
        engine.start()
    }

    private static func registerTransformComponent() {
        _ComponentRegistry.shared._register(TransformComponent())
    }
}