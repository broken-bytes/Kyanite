public extension ECS {

    enum ECSError: Error {

        case componentNotRegistered(type: Any.Type, message: String)
        case componentNotFound(type: Any.Type, message: String)
    }
}