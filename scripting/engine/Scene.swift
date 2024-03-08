@_implementationOnly import SceneManagement

public struct Scene {
    public struct Environment {

    }

    public internal(set) static var current: Scene!

    public let environment: Environment
    public let entities: [Entity]
}